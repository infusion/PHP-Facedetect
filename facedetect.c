/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Robert Eisele <robert@xarg.org>                              |
  +----------------------------------------------------------------------+
*/
//	http://www.santyago.pl/files/facedetect-1.0.1-opencv-2.2.0.patch
//	- face recog lib embedden http://libface.sourceforge.net/file/Examples.html
//	- http://www.cognotics.com/opencv/servo_2007_series/part_5/index.html
//	- opencv 2.2 kompatibel
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "ext/standard/info.h"
#include "php_facedetect.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

/* {{{ facedetect_functions[]
 *
 * Every user visible function must have an entry in facedetect_functions[].
 */
static zend_function_entry facedetect_functions[] = {
    PHP_FE(face_detect, NULL)
    PHP_FE(face_count, NULL)
    {NULL, NULL, NULL}
};
/* }}} */

/* {{{ facedetect_module_entry
 */
zend_module_entry facedetect_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_FACEDETECT_EXTNAME,
    facedetect_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_MINFO(facedetect),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_FACEDETECT_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FACEDETECT
ZEND_GET_MODULE(facedetect)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINFO_FUNCTION(facedetect)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "facedetect support", "enabled");
	php_info_print_table_row(2, "facedetect version", PHP_FACEDETECT_VERSION);
	php_info_print_table_row(2, "OpenCV version", CV_VERSION);
	php_info_print_table_end();
}
/* }}} */

static void php_facedetect(INTERNAL_FUNCTION_PARAMETERS, int return_type)
{
	char *file, *casc;
	long flen, clen;

	zval *array;

	CvHaarClassifierCascade* cascade;
	IplImage *img, *gray;
	CvMemStorage *storage;
	CvSeq *faces;
	CvRect *rect;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &file, &flen, &casc, &clen) == FAILURE) {
		RETURN_NULL();
	}

	img = cvLoadImage(file, 1);
	if(!img) {
		RETURN_FALSE;
	}

	cascade = (CvHaarClassifierCascade*)cvLoad(casc, 0, 0, 0);
	if(!cascade) {
		RETURN_FALSE;
	}

	gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY);
	cvEqualizeHist(gray, gray);

	storage = cvCreateMemStorage(0);

	faces = cvHaarDetectObjects(gray, cascade, storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(0, 0));

	if(return_type) {

		array_init(return_value);

		if(faces && faces->total > 0) {
			int i;
			for(i = 0; i < faces->total; i++) {
				MAKE_STD_ZVAL(array);
				array_init(array);

				rect = (CvRect *)cvGetSeqElem(faces, i);

				add_assoc_long(array, "x", rect->x);
				add_assoc_long(array, "y", rect->y);
				add_assoc_long(array, "w", rect->width);
				add_assoc_long(array, "h", rect->height);

				add_next_index_zval(return_value, array);
			}
		}
	} else {
		RETVAL_LONG(faces ? faces->total : 0);
	}

	cvReleaseMemStorage(&storage);
	cvReleaseImage(&gray);
	cvReleaseImage(&img);
}

/* {{{ proto array face_detect(string picture_path, string cascade_file)
   Finds coordinates of faces (or in gernal "objects") on the given picture */
PHP_FUNCTION(face_detect)
{
	php_facedetect(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto int face_count(string picture_path, string cascade_file)
   Finds number of faces (or in gernal "objects") on the given picture*/
PHP_FUNCTION(face_count)
{
	php_facedetect(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

