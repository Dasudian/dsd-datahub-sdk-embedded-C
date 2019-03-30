/*
* Copyright 2015-2016 Dasudian.com, Inc. or its affiliates. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*
* http://dsd.dasudian.com/apache2.0
*
* or in the "license" file accompanying this file. This file is distributed
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied. See the License for the specific language governing
* permissions and limitations under the License.
*/

/**
 * @file dsd_iot_tests_unit_shadow_json_builder.cpp
 * @brief IoT Client Unit Testing - Shadow JSON Builder Tests
 */

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness_c.h>

TEST_GROUP_C(ShadowJsonBuilderTests){
	TEST_GROUP_C_SETUP_WRAPPER(ShadowJsonBuilderTests)
	TEST_GROUP_C_TEARDOWN_WRAPPER(ShadowJsonBuilderTests)
};

TEST_GROUP_C_WRAPPER(ShadowJsonBuilderTests, UpdateTheJSONDocumentBuilder)
TEST_GROUP_C_WRAPPER(ShadowJsonBuilderTests, PassingNullValue)
TEST_GROUP_C_WRAPPER(ShadowJsonBuilderTests, SmallBuffer)
