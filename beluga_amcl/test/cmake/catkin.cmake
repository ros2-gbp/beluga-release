# Copyright 2023 Ekumen, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

find_package(rostest REQUIRED)

add_rostest_gtest(test_amcl_nodelet amcl_nodelet.test test_amcl_nodelet.cpp)
target_link_libraries(
  test_amcl_nodelet
  ${PROJECT_NAME}_nodelet
  ${catkin_LIBRARIES}
  gtest_main)
