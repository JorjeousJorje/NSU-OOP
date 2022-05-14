#pragma once
#include <functional>
#include "logger.hpp"



using logger_creator = std::function<std::unique_ptr<logger>(const config_t&)>;


class logger_factory {

    logger_factory() {
        creators_.emplace("", [](const config_t& config) {return  std::make_unique<logger>(config); });
        creators_.emplace("std_out", [](const config_t& config) {return  std::make_unique<console_logger>(config); });
        creators_.emplace("time_std_out", [](const config_t& config) {return std::make_unique<console_time_logger>(config); });
        creators_.emplace("file", [](const config_t& config) {return std::make_unique<file_logger>(config); });
        creators_.emplace("time_file", [](const config_t& config) {return std::make_unique<file_time_logger>(config); });
    }

    [[nodiscard]] std::unique_ptr<logger> produce(const config_t& config) const {
        const auto type = config.find("type");
    	
        const auto found = creators_.find(type->second);
        if (found != creators_.end()) {
            return found->second(config);
        }
        return std::make_unique<logger>(config);
    }

    std::unordered_map<std::string, logger_creator> creators_;

    static logger_factory& get_instance() {
        static logger_factory instance{};
        return instance;
    }

public:

    static logger& get_logger(const config_t& config = { {"type", "std_out"}, {"color", "true"}, {"log_level", "info"} }) {
        static auto singleton{ get_instance().produce(config) };
        return *singleton;
    }
};




