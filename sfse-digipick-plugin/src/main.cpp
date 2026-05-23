#include <RE/Starfield.h>
#include <REL/Relocation.h>
#include <SFSE/SFSE.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std::literals;

namespace {
	struct DigipickConfig {
		std::uint32_t keyMinSizeNovice{ 1 };
		std::uint32_t keyMinSizeAverage{ 1 };
		std::uint32_t keyMinSizeHard{ 1 };
		std::uint32_t keyMinSizeVeryHard{ 1 };
		std::uint32_t keyMaxSizeNovice{ 1 };
		std::uint32_t maxKeysNovice{ 2 };
		std::uint32_t maxKeysAverage{ 2 };
		std::uint32_t maxKeysHard{ 2 };
		std::uint32_t maxKeysVeryHard{ 2 };
		std::uint32_t maxKeysPerRingNovice{ 2 };
		std::uint32_t maxKeysPerRingVeryHard{ 2 };
		std::uint32_t puzzleShapeCountAverage{ 1 };
		std::uint32_t puzzleShapeCountHard{ 1 };
	};

	const auto kConfigPath = "Data/SFSE/Plugins/EasyDigipickSFSE.ini"sv;

	DigipickConfig g_config{};

	std::string Trim(const std::string& s)
	{
		const auto first = s.find_first_not_of(" \t\r\n");
		if (first == std::string::npos) {
			return {};
		}
		const auto last = s.find_last_not_of(" \t\r\n");
		return s.substr(first, last - first + 1);
	}

	std::optional<std::uint32_t> ParseUInt(const std::string& v)
	{
		try {
			size_t pos = 0;
			auto value = std::stoul(v, &pos, 10);
			if (pos != v.size()) {
				return std::nullopt;
			}
			return static_cast<std::uint32_t>(value);
		} catch (...) {
			return std::nullopt;
		}
	}

	void LoadConfig()
	{
		std::ifstream in(std::string(kConfigPath));
		if (!in.is_open()) {
			logger::warn("Config file not found at {}, using defaults", "Data/SFSE/Plugins/EasyDigipickSFSE.ini");
			return;
		}

		std::unordered_map<std::string, std::uint32_t*> map = {
			{ "uSecurityKeyMinSizeNovice", &g_config.keyMinSizeNovice },
			{ "uSecurityKeyMinSizeAverage", &g_config.keyMinSizeAverage },
			{ "uSecurityKeyMinSizeHard", &g_config.keyMinSizeHard },
			{ "uSecurityKeyMinSizeVeryHard", &g_config.keyMinSizeVeryHard },
			{ "uSecurityKeyMaxSizeNovice", &g_config.keyMaxSizeNovice },
			{ "uSecurityMaxKeysNovice", &g_config.maxKeysNovice },
			{ "uSecurityMaxKeysAverage", &g_config.maxKeysAverage },
			{ "uSecurityMaxKeysHard", &g_config.maxKeysHard },
			{ "uSecurityMaxKeysVeryHard", &g_config.maxKeysVeryHard },
			{ "uSecurityMaxKeysPerRingNovice", &g_config.maxKeysPerRingNovice },
			{ "uSecurityMaxKeysPerRingVeryHard", &g_config.maxKeysPerRingVeryHard },
			{ "uSecurityPuzzleShapeCountAverage", &g_config.puzzleShapeCountAverage },
			{ "uSecurityPuzzleShapeCountHard", &g_config.puzzleShapeCountHard },
		};

		std::string line;
		while (std::getline(in, line)) {
			line = Trim(line);
			if (line.empty() || line[0] == ';' || line[0] == '#') {
				continue;
			}

			auto sep = line.find('=');
			if (sep == std::string::npos) {
				continue;
			}

			auto key = Trim(line.substr(0, sep));
			auto value = Trim(line.substr(sep + 1));
			auto it = map.find(key);
			if (it == map.end()) {
				continue;
			}

			auto parsed = ParseUInt(value);
			if (!parsed.has_value()) {
				logger::warn("Invalid numeric value for {}: {}", key, value);
				continue;
			}

			*(it->second) = *parsed;
		}

		logger::info("Loaded EasyDigipickSFSE config");
	}

	bool SetGmstUInt(std::string_view name, std::uint32_t value)
	{
		auto* collection = RE::GameSettingCollection::GetSingleton();
		if (!collection) {
			logger::error("GameSettingCollection unavailable");
			return false;
		}

		auto* setting = collection->GetSetting(name.data());
		if (!setting) {
			logger::warn("GMST missing in runtime: {}", name);
			return false;
		}

		// For these uSecurity values, integer data is expected.
		setting->data.u = value;
		logger::info("Set {} = {}", name, value);
		return true;
	}

	void ApplyDigipickOverrides()
	{
		LoadConfig();

		SetGmstUInt("uSecurityKeyMinSizeNovice", g_config.keyMinSizeNovice);
		SetGmstUInt("uSecurityKeyMinSizeAverage", g_config.keyMinSizeAverage);
		SetGmstUInt("uSecurityKeyMinSizeHard", g_config.keyMinSizeHard);
		SetGmstUInt("uSecurityKeyMinSizeVeryHard", g_config.keyMinSizeVeryHard);
		SetGmstUInt("uSecurityKeyMaxSizeNovice", g_config.keyMaxSizeNovice);
		SetGmstUInt("uSecurityMaxKeysNovice", g_config.maxKeysNovice);
		SetGmstUInt("uSecurityMaxKeysAverage", g_config.maxKeysAverage);
		SetGmstUInt("uSecurityMaxKeysHard", g_config.maxKeysHard);
		SetGmstUInt("uSecurityMaxKeysVeryHard", g_config.maxKeysVeryHard);
		SetGmstUInt("uSecurityMaxKeysPerRingNovice", g_config.maxKeysPerRingNovice);
		SetGmstUInt("uSecurityMaxKeysPerRingVeryHard", g_config.maxKeysPerRingVeryHard);
		SetGmstUInt("uSecurityPuzzleShapeCountAverage", g_config.puzzleShapeCountAverage);
		SetGmstUInt("uSecurityPuzzleShapeCountHard", g_config.puzzleShapeCountHard);
	}

	void MessageHandler(SFSE::MessagingInterface::Message* msg)
	{
		if (!msg) {
			return;
		}

		if (msg->type == SFSE::MessagingInterface::kDataLoaded) {
			ApplyDigipickOverrides();
		}
	}
}

extern "C" DLLEXPORT constinit auto SFSEPlugin_Version = []() {
	SFSE::PluginVersionData v{};
	v.PluginVersion(1);
	v.PluginName("EasyDigipickSFSE");
	v.AuthorName("ChrisTitusTech + Copilot");
	v.UsesAddressLibrary(true);
	v.CompatibleVersions({ SFSE::RUNTIME_LATEST });
	return v;
}();

extern "C" DLLEXPORT bool SFSEAPI SFSEPlugin_Load(const SFSE::LoadInterface* sfse)
{
	SFSE::Init(sfse);

	auto logs = SFSE::log::log_directory();
	if (!logs) {
		return false;
	}
	*logs /= "EasyDigipickSFSE.log";
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logs->string(), true);
	auto log = std::make_shared<spdlog::logger>("global log", std::move(sink));
	spdlog::set_default_logger(std::move(log));
	spdlog::set_level(spdlog::level::info);
	spdlog::flush_on(spdlog::level::info);

	logger::info("EasyDigipickSFSE loading");

	auto* messaging = SFSE::GetMessagingInterface();
	if (!messaging) {
		logger::critical("Messaging interface unavailable");
		return false;
	}

	if (!messaging->RegisterListener(MessageHandler)) {
		logger::critical("Failed to register message listener");
		return false;
	}

	logger::info("EasyDigipickSFSE loaded");
	return true;
}
