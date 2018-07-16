/*
   Copyright (c) 2018 Timandes White

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   Author: Timandes White <timands@gmail.com>
*/

#ifndef PHP_OPENCV_H
#define PHP_OPENCV_H

extern zend_module_entry opencv_module_entry;
#define phpext_opencv_ptr &opencv_module_entry

#define PHP_OPENCV_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_OPENCV_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_OPENCV_API __attribute__ ((visibility("default")))
#else
#	define PHP_OPENCV_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(opencv)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(opencv)
*/

/* Always refer to the globals in your function as OPENCV_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define OPENCV_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(opencv, v)

#if defined(ZTS) && defined(COMPILE_DL_OPENCV)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif	/* PHP_OPENCV_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
