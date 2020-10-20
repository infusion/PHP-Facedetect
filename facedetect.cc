/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Robert Eisele (https://www.xarg.org/)                        |
  +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}
#include "ext/standard/info.h"
#include "php_facedetect.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

/* for PHP 8 */
#ifndef TSRMLS_CC
#define TSRMLS_CC
#endif

using namespace cv;

CascadeClassifier cascade;

static void php_facedetect(INTERNAL_FUNCTION_PARAMETERS, int return_type) {

#ifdef ZEND_ENGINE_3
    zval array;
#else
    zval *array;
#endif
    zval *pArray;

    char *file = NULL, *casc = NULL;
#if PHP_VERSION_ID < 70000
    long flen, clen;
#else
    size_t flen, clen;
#endif


    Mat img;
    Mat gray;
    std::vector<Rect> faces;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p|p", &file, &flen, &casc, &clen) == FAILURE) {
        RETURN_NULL();
    }

    if (access(file, R_OK) == -1) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Image file is missing or could not be read.\n");
        RETURN_FALSE;
    }

    if (casc && access(casc, R_OK) == -1) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Haar-cascade file is missing or could not be read.\n");
        RETURN_FALSE;
    }

    if (casc && !cascade.load(casc)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Haar-cascade file could not be loaded.\n");
        RETURN_FALSE;
    }

    if (!casc && cascade.empty()) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "No Haar-cascade file loaded.\n");
        RETURN_FALSE;
    }

    img = imread(file);

    if (!img.data) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Image could not be loaded.\n");
        RETURN_FALSE;
    }

    cvtColor(img, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));


    if (return_type) {

        array_init(return_value);

        for (size_t i = 0; i < faces.size(); i++) {
#ifdef ZEND_ENGINE_3
            ZVAL_NEW_ARR(&array);
            pArray = &array;
#else
            MAKE_STD_ZVAL(array);
            pArray = array;
#endif
            array_init(pArray);

            add_assoc_long(pArray, "x", faces[i].x);
            add_assoc_long(pArray, "y", faces[i].y);
            add_assoc_long(pArray, "w", faces[i].width);
            add_assoc_long(pArray, "h", faces[i].height);

            add_next_index_zval(return_value, pArray);
        }

    } else {
        RETVAL_LONG(faces.size());
    }
}

PHP_INI_MH(on_cascade_change) {

    if (ZSTR_LEN(new_value) > 0 && cascade.load(ZSTR_VAL(new_value)))
        return SUCCESS;
    else
        return FAILURE;
}

PHP_INI_BEGIN()
PHP_INI_ENTRY("facedetect.cascade", "", PHP_INI_ALL, on_cascade_change)
PHP_INI_END()


#if PHP_VERSION_ID < 80000
ZEND_BEGIN_ARG_INFO_EX(arginfo_face_detect, 0, 0, 1)
	ZEND_ARG_INFO(0, image_path)
	ZEND_ARG_INFO(0, cascade_path)
ZEND_END_ARG_INFO()

#define arginfo_face_count arginfo_face_detect

#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_face_detect, 0, 1, MAY_BE_FALSE|MAY_BE_ARRAY)
	ZEND_ARG_TYPE_INFO(0, image_path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, cascade_path, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_face_count, 0, 1, MAY_BE_FALSE|MAY_BE_LONG)
	ZEND_ARG_TYPE_INFO(0, image_path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, cascade_path, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()
#endif


PHP_FUNCTION(face_detect) {
    php_facedetect(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}

PHP_FUNCTION(face_count) {
    php_facedetect(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(facedetect) {
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(facedetect) {
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(facedetect) {
    php_info_print_table_start();
    php_info_print_table_row(2, "facedetect support", "enabled");
    php_info_print_table_row(2, "facedetect version", PHP_FACEDETECT_VERSION);
    php_info_print_table_row(2, "OpenCV version", CV_VERSION);
    php_info_print_table_end();
}
/* }}} */

/* {{{ facedetect_functions[]
 *
 * Every user visible function must have an entry in facedetect_functions[].
 */
const zend_function_entry facedetect_functions[] = {
    PHP_FE(face_detect, arginfo_face_detect)
    PHP_FE(face_count, arginfo_face_count)
    PHP_FE_END
};
/* }}} */

/* {{{ facedetect_module_entry
 */
zend_module_entry facedetect_module_entry = {
    STANDARD_MODULE_HEADER,
    "facedetect",
    facedetect_functions,
    PHP_MINIT(facedetect),
    PHP_MSHUTDOWN(facedetect),
    NULL,
    NULL,
    PHP_MINFO(facedetect),
    PHP_FACEDETECT_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FACEDETECT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
extern "C" {
    ZEND_GET_MODULE(facedetect)
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
