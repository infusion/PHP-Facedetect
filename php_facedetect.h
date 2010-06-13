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

#ifndef PHP_FACEDETECT_H
#define PHP_FACEDETECT_H

#define PHP_FACEDETECT_VERSION "1.0.1"
#define PHP_FACEDETECT_EXTNAME "facedetect"

#ifdef ZTS
# include "TSRM.h"
#endif

PHP_MINFO_FUNCTION(facedetect);

PHP_FUNCTION(face_detect);
PHP_FUNCTION(face_count);

extern zend_module_entry facedetect_module_entry;
#define phpext_facedetect_ptr &facedetect_module_entry

#endif

