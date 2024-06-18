// Copyright 2024 Ekumen, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BELUGA_SENSOR_DATA_SPARSE_VALUE_GRID_HPP
#define BELUGA_SENSOR_DATA_SPARSE_VALUE_GRID_HPP

#include <Eigen/Core>
#include <optional>
#include <type_traits>

#include <beluga/sensor/data/regular_grid.hpp>
/**
 * \file
 * \brief Implementation of generic sparse value regular grid.
 */

namespace beluga {

/// Generic 2D sparse value regular grid.
/**
 * \tparam MapType:
 *    Associative container representing a mapping from a Eigen::Vector2i to a value of type MapType::mapped_type.
 *    It should implement a subset of standard library's associative containers public API.
 *    In particular, given 'm' a possibly const instance of MapType:
 *         - MapType::key_type must be Eigen::Vector2i.
 *         - MapType::mapped_type should be the value type of the associative container entries.
 *         - 'm.at(const Eigen::Vector2i& cell_index) const' should return a const reference to 'MapType::value_type'
 * representing the value at that index, or throw 'std::out_of_range' if it doesn't exist.
 *        - 'm.find(const Eigen::Vector2i&)' should follow the same API as
 * [std::map](https://en.cppreference.com/w/cpp/container/map/find).
 */
template <typename MapType>
class SparseValueGrid : public BaseRegularGrid2<SparseValueGrid<MapType>> {
 public:
  /// Construct without data and with a 1 cell/meter resolution.
  SparseValueGrid() = default;
  /// Underlying associative container.
  using map_type = MapType;
  /// Value type of the key-value pairs stored in the data.
  using mapped_type = typename map_type::mapped_type;
  /// Key type for the key-value pairs.
  using key_type = typename map_type::key_type;
  static_assert(std::is_same_v<key_type, Eigen::Vector2i>);
  /// Constructs the grid.
  /**
   * \param data Grid data.
   * \param resolution Grid resolution.
   */
  explicit SparseValueGrid(map_type data, double resolution = 1.) : data_(std::move(data)), resolution_(resolution) {
    assert(resolution_ > 0);
  }

  /// Gets grid resolution.
  [[nodiscard]] double resolution() const { return resolution_; }

  /// Gets grid size (ie. number of grid cells).
  [[nodiscard]] std::size_t size() const { return data_.size(); }

  /// Gets grid data.
  [[nodiscard]] const map_type& data() const { return data_; }

  /// Gets grid data at cell_index or std::nullopt if it's not present.
  [[nodiscard]] std::optional<mapped_type> data_at(const Eigen::Vector2i& cell_index) const {
    auto itr = data_.find(cell_index);
    if (itr == data_.end()) {
      return std::nullopt;
    }
    return itr->second;
  }

  /// Gets grid data at real coordinates 'coordinates' or std::nullopt if it's not present.
  [[nodiscard]] std::optional<mapped_type> data_near(const Eigen::Vector2d& coordinates) const {
    return data_at(this->self().cell_near(coordinates));
  }

  /// Gets grid data at real coordinates '{x, y}' or std::nullopt if it's not present.
  [[nodiscard]] std::optional<mapped_type> data_near(const double x, const double y) const {
    return data_at(this->self().cell_near(x, y));
  }

 private:
  map_type data_;
  double resolution_ = 1.0;
};

}  // namespace beluga

#endif
