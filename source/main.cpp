#define TESLA_INIT_IMPL
#include <tesla.hpp>
#include "Utils.hpp"
#include <cstdlib>

static tsl::elm::OverlayFrame* rootFrame = nullptr;
static bool skipMain = false;

using namespace tsl;

#include "modes/FPS_Counter.hpp"
#include "modes/FPS_Graph.hpp"
#include "modes/Full.hpp"
#include "modes/Mini.hpp"
#include "modes/Micro.hpp"
#include "modes/Battery.hpp"
#include "modes/Misc.hpp"
#include "modes/Resolutions.hpp"

void InitTrans() {
	std::string jsonStr = R"(
		{
			"PluginName": "Status Monitor",
			"CPUUsageFullOverlayCustomDrawerText": "CPU Usage:",
			"GPUUsageFullOverlayCustomDrawerText": "GPU Usage:",
			"RAMUsageFullOverlayCustomDrawerText": "RAM Usage:",
			"RAMTotalUsageFullOverlayCustomDrawerText": "Total: \nApplication: \nApplet: \nSystem: \nSystem Unsafe: ",
			"ThermalUsageFullOverlayCustomDrawerText": "Board:",
			"ThermalTemperaturesDimensions1FullOverlayCustomDrawerText": "Temperatures: ",
			"ThermalTemperaturesDimensions2FullOverlayCustomDrawerText": "SoC \nPCB \nSkin ",
			"ThermalTemperaturesFullOverlayCustomDrawerText": "Temperatures: ",
			"ThermalTemperaturesItemsFullOverlayCustomDrawerText": "SoC\nPCB\nSkin",
			"GameItemsFullOverlayCustomDrawerText": "Game:",
			"HoldRefreshRateHitFullOverlayCustomDrawerText": "Hold ",
			"ExitRefreshRateHitFullOverlayCustomDrawerText": " to Exit",
			"UpdateCPUCoreFrequencyFullOverlayCustomDrawerText": "Core #0: %.2lf%%\nCore #1: %.2lf%%\nCore #2: %.2lf%%\nCore #3: %.2lf%%",
			"UpdateCPUTargetFrequencyFullOverlayCustomDrawerText": "Target Frequency: %u.%uMHz",
			"UpdateCPURealFrequencyFullOverlayCustomDrawerText": "Real Frequency: %u.%uMHz",
			"UpdateGPUTargetFrequencyFullOverlayCustomDrawerText": "Target Frequency: %u.%uMHz",
			"UpdateGPURealFrequencyFullOverlayCustomDrawerText": "Real Frequency: %u.%uMHz",
			"UpdateGPULoadFullOverlayCustomDrawerText": "Load: %u.%u%%",
			"UpdateRAMTargetFrequencyFullOverlayCustomDrawerText": "Target Frequency: %u.%uMHz",
			"UpdateRAMRealFrequencyFullOverlayCustomDrawerText": "Real Frequency: %u.%uMHz",
			"UpdateRAMLoadFullOverlayCustomDrawerText": "Load: %u.%u%% [CPU %u.%u | GPU %u.%u]",
			"UpdateFanRotationLevelFullOverlayCustomDrawerText": "Fan Rotation Level: %2.1f%%",
			"UpdateFPSTitleFullOverlayCustomDrawerText": "PFPS: %3u; FPS: %.1f",
			"UpdateOneResolutionTitleFullOverlayCustomDrawerText": "Resolutions: %dx%d",
			"UpdateResolutionTitleFullOverlayCustomDrawerText": "Resolutions: %dx%d || %dx%d",
			"UpdateReadSpeedTitleFullOverlayCustomDrawerText": "Read speed: %.2fMiB/s",
			"UpdateReadSpeedNDTitleFullOverlayCustomDrawerText": "Read speed: n/d",
			"UpdateBatteryPowerFlowFullOverlayCustomDrawerText": "Battery Power Flow: %+.2fW[%s]",
			"CPUItemMiniOverlayCustomDrawerText": "CPU",
			"GPUItemMiniOverlayCustomDrawerText": "GPU",
			"RAMItemMiniOverlayCustomDrawerText": "RAM",
			"TEMPItemMiniOverlayCustomDrawerText": "TEMP",
			"DRAWItemMiniOverlayCustomDrawerText": "DRAW",
			"CHRGItemMiniOverlayCustomDrawerText": "CHRG",
			"FANItemMiniOverlayCustomDrawerText": "FAN",
			"FPSItemMiniOverlayCustomDrawerText": "FPS",
			"RESItemMiniOverlayCustomDrawerText": "RES",
			"READItemMiniOverlayCustomDrawerText": "READ",
			"CPUItemMicroOverlayCustomDrawerText": "CPU [100%,100%,100%,100%]△4444.4",
			"GPUItemMicroOverlayCustomDrawerText": "GPU 100.0%△4444.4",
			"RAMItemDefaultMicroOverlayCustomDrawerText": "RAM 4.4/44.4GB△4444.4",
			"RAMItemMicroOverlayCustomDrawerText": "RAM 100.0%△4444.4",
			"BoardItemMicroOverlayCustomDrawerText": "BRD 88.8/88.8/88.8\u00B0C@-15.5W[99:99]",
			"FanItemMicroOverlayCustomDrawerText": "FAN 100.0%",
			"FPSItemMicroOverlayCustomDrawerText": "FPS 44.4",
			"CPUMicroOverlayCustomDrawerText": "CPU",
			"GPUMicroOverlayCustomDrawerText": "GPU",
			"RAMMicroOverlayCustomDrawerText": "RAM",
			"BRDMicroOverlayCustomDrawerText": "BRD",
			"FANMicroOverlayCustomDrawerText": "FAN",
			"FPSMicroOverlayCustomDrawerText": "FPS",
			"BatteryOrChargerStatusBatteryOverlayCustomDrawerText": "Battery/Charger Status:",
			"UpdateBetteryChargeInfoAVGBatteryOverlayCustomDrawerText": " [5s AVG]",
			"UpdateBetteryChargeInfoChargerBatteryOverlayCustomDrawerText": "Battery Actual Capacity: %.0fmAh\nBattery Designed Capacity: %.0fmAh\nBattery Temperature: %.1f\u00B0C\nBattery Raw Charge: %.1f%%\nBattery Age: %.1f%%\nBattery Voltage (%ds AVG): %.0fmV\nBattery Current Flow (%ss AVG): %+.0fmA\nBattery Power Flow%s: %+.3fW\nBattery Remaining Time: %s\nCharger Type: %u\nCharger Max Voltage: %umV\nCharger Max Current: %umA",
			"UpdateBetteryChargeInfoBetteryBatteryOverlayCustomDrawerText": "Battery Actual Capacity: %.0fmAh\nBattery Designed Capacity: %.0fmAh\nBattery Temperature: %.1f\u00B0C\nBattery Raw Charge: %.1f%%\nBattery Age: %.1f%%\nBattery Voltage (%ds AVG): %.0fmV\nBattery Current Flow (%ss AVG): %+.0fmA\nBattery Power Flow%s: %+.3fW\nBattery Remaining Time: %s",
			"MultimediaClockRatesMiscOverlayCustomDrawerText": "Multimedia clock rates:",
			"NetworkMiscOverlayCustomDrawerText": "Network",
			"NetworkTypeWifiMiscOverlayCustomDrawerText": "Type: Wi-Fi",
			"NetworkTypeWifiShowPasswordMiscOverlayCustomDrawerText": "Press Y to show password",
			"NetworkTypeEthernetMiscOverlayCustomDrawerText": "Type: Ethernet",
			"NetworkTypeNotConnectedMiscOverlayCustomDrawerText": "Type: Not connected",
			"UpdateDSPUsageMiscOverlayCustomDrawerText": "DSP usage: %u%%",
			"UpdateNVDECMiscOverlayCustomDrawerText": "NVDEC: %.1fMHz",
			"UpdateNVENCMiscOverlayCustomDrawerText": "NVJPG: %.1fMHz",
			"GraphsMenuOvMiscOverlayCustomDrawerText": "NVENC: %.1fMHz",
			"UpdateNVJPGerlayFrameText": "FPS",
			"GraphsGraphsMenuListItemText": "Graphs",
			"CounterGraphsMenuListItemText": "Counter",
			"OtherMenuOverlayFrameText": "Other",
			"BatteryOrChargerOtherMenuListItemText": "Battery/Charger",
			"MiscOtherMenMenuListItemText": "Miscellaneous",
			"GameResolutionsOtherMenMenuListItemText": "Game Resolutions",
			"FullMainMenuListItemText": "Full",
			"MiniMainMenuListItemText": "Mini",
			"MicroMainMenuListItemText": "Micro",
			"FPSMainMenuListItemText": "FPS",
			"DepthResolutionsOverlayCustomDrawerText": "Depth:",
			"ViewportResolutionsOverlayCustomDrawerText": "Viewport:",
			"GamaNotRunningOrIncompatibleResolutionsOverlayCustomDrawerText": "Game is not running or it's incompatible.",
			"OtherMainMenuListItemText": "Other"
		}
	)";
	fsdevMountSdmc();
	std::string lanPath = std::string("sdmc:/switch/.overlays/lang/") + APPTITLE + "/";
	tsl::hlp::doWithSmSession([&lanPath, &jsonStr]{
		tsl::tr::InitTrans(lanPath, jsonStr);
	});
	fsdevUnmountDevice("sdmc");
}

//Graphs
class GraphsMenu : public tsl::Gui {
public:
	GraphsMenu() {}

	virtual tsl::elm::Element* createUI() override {
		rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, "GraphsMenuOverlayFrameText"_tr);
		auto list = new tsl::elm::List();

		auto comFPSGraph = new tsl::elm::ListItem("GraphsGraphsMenuListItemText"_tr);
		comFPSGraph->setClickListener([](uint64_t keys) {
			if (keys & HidNpadButton_A) {
				tsl::changeTo<com_FPSGraph>();
				return true;
			}
			return false;
		});
		list->addItem(comFPSGraph);

		auto comFPSCounter = new tsl::elm::ListItem("CounterGraphsMenuListItemText"_tr);
		comFPSCounter->setClickListener([](uint64_t keys) {
			if (keys & HidNpadButton_A) {
				tsl::changeTo<com_FPS>();
				return true;
			}
			return false;
		});
		list->addItem(comFPSCounter);

		rootFrame->setContent(list);

		return rootFrame;
	}

	virtual void update() override {
		if (tsl::cfg::LayerPosX || tsl::cfg::LayerPosY) {
			tsl::gfx::Renderer::getRenderer().setLayerPos(0, 0);
		}
	}

	virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
		if (keysDown & HidNpadButton_B) {
			tsl::goBack();
			return true;
		}
		return false;
	}
};

//Other
class OtherMenu : public tsl::Gui {
public:
	OtherMenu() { }

	virtual tsl::elm::Element* createUI() override {
		rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, "OtherMenuOverlayFrameText"_tr);
		auto list = new tsl::elm::List();

		auto Battery = new tsl::elm::ListItem("BatteryOrChargerOtherMenuListItemText"_tr);
		Battery->setClickListener([](uint64_t keys) {
			if (keys & HidNpadButton_A) {
				tsl::changeTo<BatteryOverlay>();
				return true;
			}
			return false;
		});
		list->addItem(Battery);

		auto Misc = new tsl::elm::ListItem("MiscOtherMenMenuListItemText"_tr);
		Misc->setClickListener([](uint64_t keys) {
			if (keys & HidNpadButton_A) {
				tsl::changeTo<MiscOverlay>();
				return true;
			}
			return false;
		});
		list->addItem(Misc);

		if (SaltySD) {
			auto Res = new tsl::elm::ListItem("GameResolutionsOtherMenMenuListItemText"_tr);
			Res->setClickListener([](uint64_t keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<ResolutionsOverlay>();
					return true;
				}
				return false;
			});
			list->addItem(Res);
		}

		rootFrame->setContent(list);

		return rootFrame;
	}

	virtual void update() override {
		if (tsl::cfg::LayerPosX || tsl::cfg::LayerPosY) {
			tsl::gfx::Renderer::getRenderer().setLayerPos(0, 0);
		}
	}

	virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
		if (keysDown & HidNpadButton_B) {
			tsl::goBack();
			return true;
		}
		return false;
	}
};

//Main Menu
class MainMenu : public tsl::Gui {
public:
	MainMenu() {}

	virtual tsl::elm::Element* createUI() override {
		rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, APP_VERSION);
		auto list = new tsl::elm::List();
		
		auto Full = new tsl::elm::ListItem("FullMainMenuListItemText"_tr);
		Full->setClickListener([](uint64_t keys) {
			if (keys & HidNpadButton_A) {
				tsl::changeTo<FullOverlay>();
				return true;
			}
			return false;
		});
		list->addItem(Full);
		auto Mini = new tsl::elm::ListItem("MiniMainMenuListItemText"_tr);
		Mini->setClickListener([](uint64_t keys) {
			if (keys & HidNpadButton_A) {
				tsl::changeTo<MiniOverlay>();
				return true;
			}
			return false;
		});
		list->addItem(Mini);
		bool fileExist = false;
		tsl::hlp::doWithSDCardHandle([&fileExist] {
			FILE* test = fopen(std::string(folderpath + filename).c_str(), "rb");
			if (test) {
				fclose(test);
				fileExist = true;
				filepath = folderpath + filename;
			}
			else {
				test = fopen(std::string(folderpath + APPTITLE ".ovl").c_str(), "rb");
				if (test) {
					fclose(test);
					fileExist = true;
					filepath = folderpath + APPTITLE ".ovl";
				}
			}
		});
		if (fileExist) {
			auto Micro = new tsl::elm::ListItem("MicroMainMenuListItemText"_tr);
			Micro->setClickListener([](uint64_t keys) {
				if (keys & HidNpadButton_A) {
					tsl::setNextOverlay(filepath, "--microOverlay_");
					tsl::Overlay::get()->close();
					return true;
				}
				return false;
			});
			list->addItem(Micro);
		}
		if (SaltySD) {
			auto Graphs = new tsl::elm::ListItem("FPSMainMenuListItemText"_tr);
			Graphs->setClickListener([](uint64_t keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GraphsMenu>();
					return true;
				}
				return false;
			});
			list->addItem(Graphs);
		}
		auto Other = new tsl::elm::ListItem("OtherMainMenuListItemText"_tr);
		Other->setClickListener([](uint64_t keys) {
			if (keys & HidNpadButton_A) {
				tsl::changeTo<OtherMenu>();
				return true;
			}
			return false;
		});
		list->addItem(Other);

		rootFrame->setContent(list);

		return rootFrame;
	}

	virtual void update() override {
		if (tsl::cfg::LayerPosX || tsl::cfg::LayerPosY) {
			tsl::gfx::Renderer::getRenderer().setLayerPos(0, 0);
		}
	}

	virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
		if (keysDown & HidNpadButton_B) {
			tsl::goBack();
			return true;
		}
		return false;
	}
};

class MonitorOverlay : public tsl::Overlay {
public:
	virtual void initServices() override {
		InitTrans();
		//Initialize services
		tsl::hlp::doWithSmSession([this] {

			apmInitialize();
			if (hosversionAtLeast(8,0,0)) clkrstCheck = clkrstInitialize();
			else pcvCheck = pcvInitialize();

			if (hosversionAtLeast(5,0,0)) tcCheck = tcInitialize();

			if (hosversionAtLeast(6,0,0) && R_SUCCEEDED(pwmInitialize())) {
				pwmCheck = pwmOpenSession2(&g_ICon, 0x3D000001);
			}

			if (R_SUCCEEDED(nvInitialize())) nvCheck = nvOpen(&fd, "/dev/nvhost-ctrl-gpu");

			psmCheck = psmInitialize();
			if (R_SUCCEEDED(psmCheck)) {
				psmService = psmGetServiceSession();
			}
			i2cCheck = i2cInitialize();

			SaltySD = CheckPort();

			if (SaltySD) {
				LoadSharedMemoryAndRefreshRate();
			}
			if (sysclkIpcRunning() && R_SUCCEEDED(sysclkIpcInitialize())) {
				uint32_t sysClkApiVer = 0;
				sysclkIpcGetAPIVersion(&sysClkApiVer);
				if (sysClkApiVer < 4) {
					sysclkIpcExit();
				}
				else sysclkCheck = 0;
			}
		});
		Hinted = envIsSyscallHinted(0x6F);
	}

	virtual void exitServices() override {
		CloseThreads(true);
		if (R_SUCCEEDED(sysclkCheck)) {
			sysclkIpcExit();
		}
		shmemClose(&_sharedmemory);
		//Exit services
		clkrstExit();
		pcvExit();
		tsExit();
		tcExit();
		pwmChannelSessionClose(&g_ICon);
		pwmExit();
		nvClose(fd);
		nvExit();
		psmExit();
		i2cExit();
		apmExit();
	}

	virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
	virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

	virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
		return initially<MainMenu>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
	}
};

class MicroMode : public tsl::Overlay {
public:
	virtual void initServices() override {
		InitTrans();
		//Initialize services
		tsl::hlp::doWithSmSession([this]{
			apmInitialize();
			if (hosversionAtLeast(8,0,0)) clkrstCheck = clkrstInitialize();
			else pcvCheck = pcvInitialize();

			if (R_SUCCEEDED(nvInitialize())) nvCheck = nvOpen(&fd, "/dev/nvhost-ctrl-gpu");

			if (hosversionAtLeast(5,0,0)) tcCheck = tcInitialize();

			if (hosversionAtLeast(6,0,0) && R_SUCCEEDED(pwmInitialize())) {
				pwmCheck = pwmOpenSession2(&g_ICon, 0x3D000001);
			}

			i2cCheck = i2cInitialize();

			psmCheck = psmInitialize();
			if (R_SUCCEEDED(psmCheck)) {
				psmService = psmGetServiceSession();
			}

			SaltySD = CheckPort();

			if (SaltySD) {
				LoadSharedMemory();
			}
			if (sysclkIpcRunning() && R_SUCCEEDED(sysclkIpcInitialize())) {
				uint32_t sysClkApiVer = 0;
				sysclkIpcGetAPIVersion(&sysClkApiVer);
				if (sysClkApiVer < 4) {
					sysclkIpcExit();
				}
				else sysclkCheck = 0;
			}
		});
		Hinted = envIsSyscallHinted(0x6F);
	}

	virtual void exitServices() override {
		CloseThreads(true);
		shmemClose(&_sharedmemory);
		if (R_SUCCEEDED(sysclkCheck)) {
			sysclkIpcExit();
		}
		//Exit services
		clkrstExit();
		pcvExit();
		tsExit();
		tcExit();
		pwmChannelSessionClose(&g_ICon);
		pwmExit();
		i2cExit();
		psmExit();
		nvClose(fd);
		nvExit();
		apmExit();
	}

	virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
	virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

	virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
		return initially<MicroOverlay>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
	}
};

// This function gets called on startup to create a new Overlay object
int main(int argc, char **argv) {
	#if !defined(__SWITCH__) && !defined(__OUNCE__)
		systemtickfrequency = armGetSystemTickFreq();
	#endif

	tsl::hlp::doWithSDCardHandle([] {
		ParseIniFile(); // parse INI from file
	});

	if (argc > 0) {
		filename = argv[0];
	}
	for (u8 arg = 0; arg < argc; arg++) {
		if (strcasecmp(argv[arg], "--microOverlay_") == 0) {
			framebufferWidth = 1280;
			framebufferHeight = 28;
			tsl::hlp::doWithSDCardHandle([] {
				FILE* test = fopen(std::string(folderpath + filename).c_str(), "rb");
				if (test) {
					fclose(test);
					filepath = folderpath + filename;
				}
				else {
					test = fopen(std::string(folderpath + APPTITLE ".ovl").c_str(), "rb");
					if (test) {
						fclose(test);
						filepath = folderpath + APPTITLE ".ovl";
					}
				}
			});
			return tsl::loop<MicroMode>(argc, argv);
		} else if (strcasecmp(argv[arg], "--microOverlay") == 0) {
			skipMain = true;
			framebufferWidth = 1280;
			framebufferHeight = 28;
			tsl::hlp::doWithSDCardHandle([] {
				FILE* test = fopen(std::string(folderpath + filename).c_str(), "rb");
				if (test) {
					fclose(test);
					filepath = folderpath + filename;
				}
				else {
					test = fopen(std::string(folderpath + APPTITLE ".ovl").c_str(), "rb");
					if (test) {
						fclose(test);
						filepath = folderpath + APPTITLE ".ovl";
					}
				}
			});
			return tsl::loop<MicroMode>(argc, argv);
		}
	}
	return tsl::loop<MonitorOverlay>(argc, argv);
}