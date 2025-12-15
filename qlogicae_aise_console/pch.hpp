#pragma once

#include <CLI/CLI.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include <absl/strings/str_cat.h>
#include <absl/strings/str_replace.h>

#include <indicators/cursor_control.hpp>
#include <indicators/progress_spinner.hpp>
#include <indicators/block_progress_bar.hpp>

#include <nlohmann/json.hpp>
#include <onnxruntime_cxx_api.h>

#include <any>
#include <compare>
#include <variant>
#include <optional>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <source_location>

#include <bit>
#include <limits>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <climits>

#include <regex>
#include <string>
#include <format>
#include <cctype>
#include <cwctype>
#include <charconv>
#include <string_view>

#include <set>
#include <map>
#include <list>
#include <span>
#include <array>
#include <deque>
#include <tuple>
#include <stack>
#include <queue>
#include <vector>
#include <ranges>
#include <iterator>
#include <unordered_set>
#include <unordered_map>

#include <new>
#include <memory>
#include <memory_resource>

#include <mutex>
#include <latch>
#include <thread>
#include <future>
#include <atomic>
#include <barrier>
#include <semaphore>
#include <stop_token>
#include <shared_mutex>
#include <condition_variable>

#include <ratio>
#include <chrono>
#include <random>
#include <numbers>

#include <fstream>
#include <filesystem>

#include <ios>
#include <cstdio>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <iostream>

#include <cmath>
#include <cerrno>
#include <cassert>
#include <complex>
#include <csignal>
#include <valarray>
#include <stdexcept>

#include <locale>
#include <execution>
#include <algorithm>
#include <functional>

#include <version>

#include <concepts>
#include <coroutine>

#include <cfenv>
#include <cfloat>

#include <ctime>
#include <cstdarg>
#include <csetjmp>
#include <cstring>

#include <windows.h>

#include "qlogicae_core/includes/result.hpp"
#include "qlogicae_core/includes/logger.hpp"
#include "qlogicae_core/includes/cli_io.hpp"
#include "qlogicae_core/includes/utilities.hpp"
#include "qlogicae_core/includes/application.hpp"
#include "qlogicae_core/includes/file_system.hpp"
#include "qlogicae_core/includes/transformer.hpp"
#include "qlogicae_core/includes/text_file_io.hpp"
#include "qlogicae_core/includes/json_file_io.hpp"
#include "qlogicae_core/includes/log_configurations.hpp"
#include "qlogicae_core/includes/application_utilities.hpp"

#include "qlogicae_aise_core/includes/aise_api.hpp"
#include "qlogicae_aise_core/includes/aise_api_configurations.hpp"
#include "qlogicae_aise_core/includes/aise_api_file_evaluation_results.hpp"
#include "qlogicae_aise_core/includes/aise_api_file_line_evaluation_results.hpp"
#include "qlogicae_aise_core/includes/aise_api_file_system_evaluation_results.hpp"
#include "qlogicae_aise_core/includes/aise_api_file_system_evaluation_configurations.hpp"


namespace QLogicaeAiseConsole
{

}
