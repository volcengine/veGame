#include "cloudrenderx_demo_config.h"
#include <Windows.h>
#include <fstream>
#include "ve_log_lite.h"
#include <map>

static std::map<std::string, std::shared_ptr<cloudrenderx_demo::CloudrenderxDemoConfig>> all_configs;

static bool has_already_parsed_config_ini_file = false;

cloudrenderx_demo::CloudrenderxDemoConfig::CloudrenderxDemoConfig(
	const std::string& _id, 
	const std::string& _ak, 
	const std::string& _sk, 
	const std::string& _token,
	const std::string& _game_id) : account_id(_id), ak(_ak), sk(_sk), token(_token), game_id(_game_id)
{
}

nlohmann::json parseConfigFromFile()
{
    char modulePath[MAX_PATH];
    DWORD result = GetModuleFileNameA(NULL, modulePath, MAX_PATH);
    // 提取目录部分
    std::string exeDirectory = modulePath;
    size_t lastSlash = exeDirectory.find_last_of("\\/");
    if (lastSlash != std::wstring::npos)
    {
        exeDirectory = exeDirectory.substr(0, lastSlash);
    }

    // 构建config.ini的路径
    std::string path = exeDirectory + "\\config.ini";

    std::ifstream ifs(path.c_str(), std::ios::in);
    if (!ifs.is_open()) {
        veLOGE("cloud not find the config file[{}]...", path.c_str());
        abort();
    }
    std::istreambuf_iterator<char> beg(ifs), end;
    auto config_content = std::string(beg, end);
    veLOGI("the content of the config.ini file is {}", config_content);

    //移动的文件尾部
    ifs.seekg(0, std::ios::end);

    ifs.close();

    return nlohmann::json::parse(config_content);
}


// 从demo exe同目录下的config.ini文件解析配置信息
// return的map中key表示配置名，value表示具体的配置实例
std::map<std::string, std::shared_ptr<cloudrenderx_demo::CloudrenderxDemoConfig>>& getVeDemoConfigFromConfigFile()
{
	if (has_already_parsed_config_ini_file) {
		return all_configs;
	}

	has_already_parsed_config_ini_file = true;

    auto config_json = parseConfigFromFile();
    if (config_json.empty()) {
        veLOGE("the config of the vegame demo is empty, it must not be empty...");
        abort();
    }

    std::string key = "";
    for (const auto& item : config_json.items())
    {
        if (key.empty()) {
            key = item.key();
        }

        auto& json_value = config_json[key].get<nlohmann::json>();
        std::string account_id = "";
        std::string ak = "";
        std::string sk = "";
        std::string token = "";
        std::string game_id = "";
        for (const auto& item_1 : json_value.items()) {
            if (item_1.key() == "id"
                && item_1.value().is_string()) {
                account_id = item_1.value().get<std::string>();
            }
            else if (item_1.key() == "ak"
                && item_1.value().is_string()) {
                ak = item_1.value().get<std::string>();
            }
            else if (item_1.key() == "sk"
                && item_1.value().is_string()) {
                sk = item_1.value().get<std::string>();
            }
            else if (item_1.key() == "token"
                && item_1.value().is_string()) {
                token = item_1.value().get<std::string>();
            }
            else if (item_1.key() == "gameId"
                && item_1.value().is_string()) {
                game_id = item_1.value().get<std::string>();
            }
        }
        if (account_id.empty()) {
            veLOGE("the account id in config which key is {} is empty, it must not be empty...", key);
            abort();
        }

        if (ak.empty()) {
            veLOGE("the ak in config which key is {} is empty, it must not be empty...", key);
            abort();
        }

        if (sk.empty()) {
            veLOGE("the sk in config which key is {} is empty, it must not be empty...", key);
            abort();
        }

        if (token.empty()) {
            veLOGE("the account token in config which key is {} is empty, it must not be empty...", key);
            abort();
        }

        if (game_id.empty()) {
            veLOGE("the game id in config which key is {} is empty, it must not be empty...", key);
            abort();
        }

        auto config = std::make_shared<cloudrenderx_demo::CloudrenderxDemoConfig>(account_id, ak, sk, token, game_id);
        all_configs[key] = config;
    }

	return all_configs;
}

std::vector<std::string> cloudrenderx_demo::getAllConfigNames()
{
    const auto& all_configs = getVeDemoConfigFromConfigFile();

    std::vector<std::string> result;
    auto it = all_configs.begin();
    while (it != all_configs.end())
    {
        result.push_back(it->first);
        it++;
    }

    return result;
}
std::shared_ptr<cloudrenderx_demo::CloudrenderxDemoConfig> cloudrenderx_demo::getDemoConfigByName(const std::string& config_name) {
    if (config_name.empty()) {
        return nullptr;
    }

    const auto& all_configs = getVeDemoConfigFromConfigFile();
    auto it = all_configs.find(config_name);
    if (it == all_configs.end()) {
        return nullptr;
    }
    else {
        return it->second;
    }
}