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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_opencv.h"
#include "opencv2/opencv.hpp"
#include <mat.h>

static ZEND_RSRC_DTOR_FUNC(php_opencv_mat_dtor)
{
	php_opencv_mat_t *mat_p;
	
	if (!res->ptr)
		return;

	mat_p = (php_opencv_mat_t *)res->ptr;
	if (mat_p->cv_mat_p)
		delete mat_p->cv_mat_p;
	efree(mat_p);

	res->ptr = NULL;
}

/* If you declare any globals in php_opencv.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(opencv)
*/

int le_opencv_mat;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("opencv.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_opencv_globals, opencv_globals)
	STD_PHP_INI_ENTRY("opencv.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_opencv_globals, opencv_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_opencv_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_opencv_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "opencv", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_opencv_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_opencv_init_globals(zend_opencv_globals *opencv_globals)
{
	opencv_globals->global_value = 0;
	opencv_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(opencv)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

	le_opencv_mat = zend_register_list_destructors_ex(php_opencv_mat_dtor, NULL, PHP_OPENCV_RES_MAT, module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(opencv)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(opencv)
{
#if defined(COMPILE_DL_OPENCV) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(opencv)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(opencv)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "opencv support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ opencv_functions[]
 *
 * Every user visible function must have an entry in opencv_functions[].
 */
const zend_function_entry opencv_functions[] = {
    PHP_FE(opencv_mat_dump,  NULL)
    PHP_FE(opencv_inpaint,  NULL)
    PHP_FE(opencv_mat_rows,  NULL)
    PHP_FE(opencv_mat_cols,  NULL)
	PHP_FE(opencv_rectangle,  NULL)
	PHP_FE(opencv_mat_zeros,  NULL)
	PHP_FE(opencv_imwrite,  NULL)
	PHP_FE(opencv_imread,  NULL)
	PHP_FE(confirm_opencv_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in opencv_functions[] */
};
/* }}} */

/* {{{ opencv_module_entry
 */
zend_module_entry opencv_module_entry = {
	STANDARD_MODULE_HEADER,
	"opencv",
	opencv_functions,
	PHP_MINIT(opencv),
	PHP_MSHUTDOWN(opencv),
	PHP_RINIT(opencv),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(opencv),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(opencv),
	PHP_OPENCV_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_OPENCV
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(opencv)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
