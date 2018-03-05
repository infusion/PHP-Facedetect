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
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_FACEDETECT_H
#define PHP_FACEDETECT_H

extern zend_module_entry facedetect_module_entry;
#define phpext_facedetect_ptr &facedetect_module_entry

#define PHP_FACEDETECT_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_FACEDETECT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_FACEDETECT_API __attribute__ ((visibility("default")))
#else
#	define PHP_FACEDETECT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define FACEDETECT_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(facedetect, v)

#if defined(ZTS) && defined(COMPILE_DL_FACEDETECT)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_FACEDETECT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
