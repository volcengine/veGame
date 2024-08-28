#pragma once

#include <string>
#include <memory>
#include <vector>
#include "json.hpp"

namespace cloudrenderx_demo {
	class CloudrenderxDemoConfig {
	public:

		const std::string account_id;

		const std::string ak;

		const std::string sk;

		const std::string token;

		const std::string game_id;

		CloudrenderxDemoConfig(
			const std::string& _id,
			const std::string& _ak,
			const std::string& _sk,
			const std::string& _token,
			const std::string& _game_id);
	};

	std::vector<std::string> getAllConfigNames();

	std::shared_ptr<CloudrenderxDemoConfig> getDemoConfigByName(const std::string& config_name);
}