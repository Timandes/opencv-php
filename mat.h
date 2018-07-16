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

#ifndef PHP_OPENCV_MAT_H
#define PHP_OPENCV_MAT_H

#define PHP_OPENCV_RES_MAT "opencv matrix"

/* True global resources - no need for thread safety here */
extern int le_opencv_mat;

struct php_opencv_mat_s
{
	cv::Mat *cv_mat_p;
	zend_resource *z_res_p;
};

typedef php_opencv_mat_s php_opencv_mat_t;

PHP_FUNCTION(opencv_imread);
PHP_FUNCTION(opencv_imwrite);
PHP_FUNCTION(opencv_rectangle);
PHP_FUNCTION(opencv_inpaint);
PHP_FUNCTION(opencv_mat_zeros);
PHP_FUNCTION(opencv_mat_rows);
PHP_FUNCTION(opencv_mat_cols);
PHP_FUNCTION(opencv_mat_dump);

#endif /* PHP_OPENCV_MAT_H */