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
#include "Zend/zend_exceptions.h"
#include "opencv2/opencv.hpp"
#include <mat.h>

using namespace std;

#define PHP_OPENCV_FETCH_MAT_RES(mat_p, z_mat_p)							\
	mat_p = (php_opencv_mat_t *)zend_fetch_resource(Z_RES_P(z_mat_p), PHP_OPENCV_RES_MAT, le_opencv_mat); \
	if (NULL == mat_p) {													\
		zend_throw_exception(zend_ce_exception, "No such resource", -9998);	\
		return;																\
	}
#define PHP_OPENCV_CATCH_CV_EXCEPTION	\
	catch (cv::Exception ex) {			\
		zend_throw_exception(zend_ce_exception, ex.err.c_str(), ex.code);	\
		return;							\
	}
#define PHP_OPENCV_FUNCTION_MAT(PROPERTY)	\
	PHP_FUNCTION(opencv_mat_##PROPERTY)	\
	{										\
		zval *z_mat_p;						\
		php_opencv_mat_t *mat_p;			\
\
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &z_mat_p) == FAILURE) {	\
			return;							\
		}									\
\
		PHP_OPENCV_FETCH_MAT_RES(mat_p, z_mat_p);	\
\
		RETURN_LONG(mat_p->cv_mat_p->PROPERTY);	\
	}
#define PHP_OPENCV_CV_MAT_P(mat_p)	mat_p->cv_mat_p

static php_opencv_mat_t *php_opencv_mat_init(cv::Mat cv_mat)
{
	php_opencv_mat_t *retval_p;

	retval_p = (php_opencv_mat_t *)emalloc(sizeof(php_opencv_mat_t));
	retval_p->cv_mat_p = new cv::Mat(cv_mat);
	retval_p->z_res_p = zend_register_resource(retval_p, le_opencv_mat);

	return retval_p;
}

PHP_FUNCTION(opencv_imread)
{
	php_opencv_mat_t *mat;
	zend_string *path;
	cv::String cv_path;
	cv::Mat cv_mat;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &path) == FAILURE) {
		return;
	}

	cv_path = cv::String(ZSTR_VAL(path), ZSTR_LEN(path));
	cv_mat = cv::imread(cv_path);
	if (NULL == cv_mat.data) {
		zend_throw_exception(zend_ce_exception, "Found error when calling cv::imread", -9999);
		return;
	}

	mat = php_opencv_mat_init(cv_mat);

	RETURN_RES(mat->z_res_p);
}

PHP_FUNCTION(opencv_imwrite)
{
	zend_string *path_p;
	zval *z_mat_p;
	cv::String cv_path;
	php_opencv_mat_t *mat_p;
	bool retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sr", &path_p, &z_mat_p) == FAILURE) {
		return;
	}

	PHP_OPENCV_FETCH_MAT_RES(mat_p, z_mat_p);

	cv_path = cv::String(ZSTR_VAL(path_p), ZSTR_LEN(path_p));
	try {
		retval = cv::imwrite(cv_path, *(mat_p->cv_mat_p));
	} PHP_OPENCV_CATCH_CV_EXCEPTION

	if (retval) {
		RETURN_TRUE;
	}
	else {
		RETURN_FALSE;
	}
}

PHP_FUNCTION(opencv_mat_zeros)
{
	long rows, cols;
	php_opencv_mat_t *mat_p;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &rows, &cols) == FAILURE) {
		return;
	}

	mat_p = php_opencv_mat_init(cv::Mat::zeros((int)rows, (int)cols, CV_8U));

	RETURN_RES(mat_p->z_res_p);
}

cv::Point php_opencv_array_2_point(const char *param_name, zval *z_array_p)
{
	Bucket *bucket;
	char buf[128];
	long a[2];
	int i;
	zval *z_value_p;

	if (zend_hash_num_elements(Z_ARR_P(z_array_p)) < 2) {
		sprintf(buf, "%s must have at least 2 elements", param_name);
		zend_throw_exception(zend_ce_exception, buf, -9997);
		return cv::Point();
	}

	i = 0;
	ZEND_HASH_FOREACH_VAL(Z_ARR_P(z_array_p), z_value_p)
		if (Z_TYPE_P(z_value_p) != IS_LONG) {
			sprintf(buf, "%s must only have integer elements", param_name);
			zend_throw_exception(zend_ce_exception, buf, -9996);
			return cv::Point();
		}
		a[i++] = Z_LVAL_P(z_value_p);
	ZEND_HASH_FOREACH_END();

	return cv::Point(a[0], a[1]);
}

cv::Scalar php_opencv_array_2_scalar(const char *param_name, zval *z_array_p)
{
	Bucket *bucket;
	char buf[128];
	long a[4];
	int i;
	zval *z_value_p;

	if (zend_hash_num_elements(Z_ARR_P(z_array_p)) < 4) {
		sprintf(buf, "%s must have at least 4 elements", param_name);
		zend_throw_exception(zend_ce_exception, buf, -9997);
		return cv::Scalar();
	}

	i = 0;
	ZEND_HASH_FOREACH_VAL(Z_ARR_P(z_array_p), z_value_p)
		if (Z_TYPE_P(z_value_p) != IS_LONG) {
			sprintf(buf, "%s must only have integer elements", param_name);
			zend_throw_exception(zend_ce_exception, buf, -9996);
			return cv::Scalar();
		}
		a[i++] = Z_LVAL_P(z_value_p);
	ZEND_HASH_FOREACH_END();

	return CV_RGB(a[0], a[1], a[2]); // R/G/B
}

PHP_FUNCTION(opencv_rectangle)
{
	zval *z_mat_p, *z_pt1_p, *z_pt2_p, *z_color_p;
	long thickness, line_type;
	Bucket *bucket;
	cv::Point cv_pt1, cv_pt2;
	cv::Scalar cv_color;
	php_opencv_mat_t *mat_p;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "raaall"
			, &z_mat_p, &z_pt1_p, &z_pt2_p, &z_color_p, &thickness, &line_type) == FAILURE) {
		return;
	}

	PHP_OPENCV_FETCH_MAT_RES(mat_p, z_mat_p);

	try {
		// Inner matrix will be changed
		cv::rectangle(*(mat_p->cv_mat_p), cv_pt1, cv_pt2, cv_color, (int)thickness, (int)line_type);
	} PHP_OPENCV_CATCH_CV_EXCEPTION
}

PHP_OPENCV_FUNCTION_MAT(rows)
PHP_OPENCV_FUNCTION_MAT(cols)

PHP_FUNCTION(opencv_inpaint)
{
	zval *z_src_p, *z_mask_p;
	php_opencv_mat_t *src_p, *mask_p;
	cv::Mat dst;
	double radius;
	php_opencv_mat_t *retval_p;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rrd", &z_src_p, &z_mask_p, &radius) == FAILURE) {
		return;
	}

	PHP_OPENCV_FETCH_MAT_RES(src_p, z_src_p);
	PHP_OPENCV_FETCH_MAT_RES(mask_p, z_mask_p);

	try {
		cv::inpaint(*PHP_OPENCV_CV_MAT_P(src_p), *PHP_OPENCV_CV_MAT_P(mask_p), dst, radius, cv::INPAINT_TELEA);
	} PHP_OPENCV_CATCH_CV_EXCEPTION;

	retval_p = php_opencv_mat_init(dst);

	RETURN_RES(retval_p->z_res_p);
}

PHP_FUNCTION(opencv_mat_dump)
{
	zval *z_mat_p;
	php_opencv_mat_t *mat_p;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &z_mat_p) == FAILURE) {
		return;
	}

	PHP_OPENCV_FETCH_MAT_RES(mat_p, z_mat_p);

	cout << *PHP_OPENCV_CV_MAT_P(mat_p) << endl;
}