#define TESLA_INIT_IMPL
#include <tesla.hpp>
#include "Utils.hpp"

using namespace tsl;

void InitTrans() {
    std::string jsonStr = R"(
        {
            "PluginName": "Status Monitor",
            "CPUUsageFullOverlayCustomDrawerText": "CPU Usage:",
            "GPUUsageFullOverlayCustomDrawerText": "GPU Usage:",
            "MEMUsageFullOverlayCustomDrawerText": "RAM Usage:",
            "ThermalUsageFullOverlayCustomDrawerText": "Board:",
            "TemperaturesFullOverlayCustomDrawerText": "Temperatures: SoC\n\t\t\t\t\t\t\t PCB\n\t\t\t\t\t\t\t Skin",
            "DecreaseRefreshRateHitFullOverlayCustomDrawerText": "Hold Left Stick & Right Stick to Exit\nHold ZR + R + D-Pad Down to slow down refresh",
            "IncreaseRefreshRateHitFullOverlayCustomDrawerText": "Hold Left Stick & Right Stick to Exit\nHold ZR + R + D-Pad Up to speed up refresh",
            "UpdateCPUFreqFullOverlayCustomDrawerText": "Frequency: %.1f MHz",
            "UpdateCPUCore0FullOverlayCustomDrawerText": "Core #0: %.2f%s",
            "UpdateCPUCore1FullOverlayCustomDrawerText": "Core #1: %.2f%s",
            "UpdateCPUCore2FullOverlayCustomDrawerText": "Core #2: %.2f%s",
            "UpdateCPUCore3FullOverlayCustomDrawerText": "Core #3: %.2f%s",
            "UpdateGPUFreqFullOverlayCustomDrawerText": "Frequency: %.1f MHz",
            "UpdateGPULoadFullOverlayCustomDrawerText": "Load: %.1f%s",
            "UpdateMEMFreqFullOverlayCustomDrawerText": "Frequency: %.1f MHz",
            "UpdateMEMSizeFullOverlayCustomDrawerText": "Total:",
            "UpdateMEMAppFullOverlayCustomDrawerText": "Application:",
            "UpdateMEMAppletFullOverlayCustomDrawerText": "Applet:",
            "UpdateMEMSystemFullOverlayCustomDrawerText": "System:",
            "UpdateMEMSystemReservedFullOverlayCustomDrawerText": "System Unsafe:",
            "UpdateBatteryPowerFlowFullOverlayCustomDrawerText": "Battery Power Flow:\t%+.2fW",
            "UpdateThermalHos14FullOverlayCustomDrawerText": "%2d \u00B0C\n%2d \u00B0C\n%2.2f \u00B0C",
            "UpdateThermalFullOverlayCustomDrawerText": "%2.2f \u00B0C\n%2.2f \u00B0C\n%2.2f \u00B0C",
            "UpdateFanRotationLevelFullOverlayCustomDrawerText": "Fan Rotation Level:\t%2.2f%s",
            "UpdatePFPSFullOverlayCustomDrawerText": "PFPS:",
            "UpdateFPSFullOverlayCustomDrawerText": "FPS:",
            "GameRunningMiniOverlayCustomDrawerText": "CPU\nGPU\nRAM\nTEMP\nFAN\nDRAW\nPFPS\nFPS",
            "NoGameRunningMiniOverlayCustomDrawerText": "CPU\nGPU\nRAM\nTEMP\nFAN\nDRAW",
            "UpdateFPSMiniOverlayCustomDrawerText": "PFPS:",
            "UpdateFPSAvgMiniOverlayCustomDrawerText": "FPS:",
            "NoGameRunningCPUMicroOverlayCustomDrawerText": "CPU",
            "NoGameRunningGPUMicroOverlayCustomDrawerText": "GPU",
            "NoGameRunningRAMMicroOverlayCustomDrawerText": "RAM",
            "NoGameRunningBRDMicroOverlayCustomDrawerText": "BRD",
            "NoGameRunningFANMicroOverlayCustomDrawerText": "FAN",
            "GameRunningCPUMicroOverlayCustomDrawerText": "CPU",
            "GameRunningGPUMicroOverlayCustomDrawerText": "GPU",
            "GameRunningRAMMicroOverlayCustomDrawerText": "RAM",
            "GameRunningBRDMicroOverlayCustomDrawerText": "BRD",
            "GameRunningFANMicroOverlayCustomDrawerText": "FAN",
            "GameRunningFPSMicroOverlayCustomDrawerText": "FPS",
            "BatteryOrChargerUsageBatteryOverlayCustomDrawerText": "Battery/Charger Stats:",
            "UpdateBetteryChargeInfoChargerBatteryOverlayCustomDrawerText": "Battery Temperature: %.1f\u00B0C\nBattery Raw Charge: %.1f%s\nBattery Age: %.1f%s\nBattery Voltage (5s AVG): %.0f mV\nBattery Current Flow (5s AVG): %+.0f mA\nBattery Power Flow (5s AVG): %+.3f W\nCharger Type: %s\nCharger Max Voltage: %u mV\nCharger Max Current: %u mA",
            "UpdateBetteryChargeInfoBetteryBatteryOverlayCustomDrawerText": "Battery Temperature: %.1f\u00B0C\nBattery Raw Charge: %.1f%s\nBattery Age: %.1f%s\nBattery Voltage (5s AVG): %.0f mV\nBattery Current Flow (5s AVG): %.0f mA\nBattery Power Flow (5s AVG): %+.3f W",
            "NetworkMiscOverlayCustomDrawerText": "Network",
            "NetworkTypeWifiMiscOverlayCustomDrawerText": "Type: Wi-Fi",
            "NetworkTypeWifiShowPasswordMiscOverlayCustomDrawerText": "Press Y to show password",
            "NetworkTypeEthernetMiscOverlayCustomDrawerText": "Type: Ethernet",
            "NetworkTypeNotConnectedMiscOverlayCustomDrawerText": "Type: Not connected",
            "UpdateDSPUsageMiscOverlayCustomDrawerText": "DSP usage: %u%%",
            "UpdateNVDECClockRateMiscOverlayCustomDrawerText": "NVDEC clock rate: %.1f MHz",
            "GraphsMenuListItemText": "Graphs",
            "FPSGraphsMenuListItemText": "FPS",
            "OtherMenuListItemText": "Other",
            "BatteryOrChargerOtherMenuListItemText": "Battery/Charger",
            "MiscOtherMenuListItemText": "Miscellaneous",
            "FullMainMenuListItemText": "Full",
            "MiniMainMenuListItemText": "Mini",
            "MicroMainMenuListItemText": "Micro",
            "FPSMainMenuListItemText": "FPS Counter",
            "GraphsMainMenuListItemText": "Graphs",
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

//FPS Counter mode
class com_FPS : public tsl::Gui {
public:
    com_FPS() { }
    s16 base_y = 0;
    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("", "");
        auto Status = new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
                static uint8_t avg = 0;
                if (FPSavg >= 100) avg = 46;
                else if (FPSavg >= 10) avg = 23;
                else avg = 0;
                renderer->drawRect(0, base_y, tsl::cfg::FramebufferWidth - 370 + avg, 50, a(0x7111));
                renderer->drawString(FPSavg_c, false, 5, base_y+40, 40, renderer->a(0xFFFF));
        });

        rootFrame->setContent(Status);
        return rootFrame;
    }

    virtual void update() override {
        ///FPS
        snprintf(FPSavg_c, sizeof FPSavg_c, "%2.1f", FPSavg);
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if ((keysHeld & HidNpadButton_StickL) && (keysHeld & HidNpadButton_StickR)) {
            EndFPSCounterThread();
            tsl::goBack();
            return true;
        } else if ((keysHeld & HidNpadButton_ZR) && (keysHeld & HidNpadButton_R)) {
            if ((keysHeld & HidNpadButton_Up) && base_y != 0) {
                base_y = 0;
            } else if ((keysHeld & HidNpadButton_Down) && base_y != 670) {
                base_y = 670;
            }
        }
        return false;
    }
};

//FPS Counter mode
class com_FPSGraph : public tsl::Gui {
public:
    com_FPSGraph() { }

    struct stats {
        s16 value;
        bool zero_rounded;
    };

    std::vector<stats> readings;

    s16 base_y = 0;
    s16 rectangle_width = 180;
    s16 rectangle_height = 60;
    s16 rectangle_x = 15;
    s16 rectangle_y = 5;
    s16 rectangle_range_max = 60;
    s16 rectangle_range_min = 0;
    char legend_max[3] = "60";
    char legend_min[2] = "0";
    s32 range = std::abs(rectangle_range_max - rectangle_range_min) + 1;
    s16 x_end = rectangle_x + rectangle_width;
    s16 y_old = rectangle_y+rectangle_height;
    s16 y_30FPS = rectangle_y+(rectangle_height / 2);
    s16 y_60FPS = rectangle_y;
    bool isAbove = false;

    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("", "");

        auto Status = new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
            renderer->drawRect(0, base_y, 201, 72, a(0x7111));
            if (FPSavg < 10) {
                renderer->drawString(FPSavg_c, false, 55, base_y + 60, 63, renderer->a(0x4444));
            } else if (FPSavg < 100) {
                renderer->drawString(FPSavg_c, false, 35, base_y + 60, 63, renderer->a(0x4444));
            } else
                renderer->drawString(FPSavg_c, false, 15, base_y + 60, 63, renderer->a(0x4444));
            renderer->drawEmptyRect(rectangle_x - 1, base_y + rectangle_y - 1, rectangle_width + 2, rectangle_height + 4, renderer->a(0xF77F));
            renderer->drawDashedLine(rectangle_x, base_y + y_30FPS, rectangle_x+rectangle_width, base_y + y_30FPS, 6, renderer->a(0x8888));
            renderer->drawString(&legend_max[0], false, rectangle_x - 15, base_y + rectangle_y + 7, 10, renderer->a(0xFFFF));
            renderer->drawString(&legend_min[0], false, rectangle_x - 10, base_y + rectangle_y + rectangle_height + 3, 10, renderer->a(0xFFFF));

            size_t last_element = readings.size() - 1;

            for (s16 x = x_end; x > static_cast<s16>(x_end-readings.size()); x--) {
                s32 y_on_range = readings[last_element].value + std::abs(rectangle_range_min) + 1;
                if (y_on_range < 0) {
                    y_on_range = 0;
                } else if (y_on_range > range) {
                    isAbove = true;
                    y_on_range = range;
                }

                s16 y = rectangle_y + static_cast<s16>(std::lround((float)rectangle_height * ((float)(range - y_on_range) / (float)range))); // 320 + (80 * ((61 - 61)/61)) = 320
                auto colour = renderer->a(0xFFFF);
                if (y == y_old && !isAbove && readings[last_element].zero_rounded) {
                    if ((y == y_30FPS || y == y_60FPS))
                        colour = renderer->a(0xF0C0);
                    else
                        colour = renderer->a(0xFF0F);
                }

                if (x == x_end) {
                    y_old = y;
                } /*else if (y - y_old > 0) {
                    if (y_old + 1 <= rectangle_y+rectangle_height)
                        y_old += 1;
                } else if (y - y_old < 0) {
                    if (y_old - 1 >= rectangle_y)
                        y_old -= 1;
                }
                */

                renderer->drawLine(x, base_y + y, x, base_y + y_old, colour);
                isAbove = false;
                y_old = y;
                last_element--;
            }

        });

        rootFrame->setContent(Status);

        return rootFrame;
    }

    virtual void update() override {
        ///FPS
        static float FPSavg_old = 0;
        stats temp = {0, false};

        if (FPSavg_old == FPSavg)
            return;
        FPSavg_old = FPSavg;
        snprintf(FPSavg_c, sizeof FPSavg_c, "%2.1f",  FPSavg);
        if (FPSavg < 254) {
            if ((s16)(readings.size()) >= rectangle_width) {
                readings.erase(readings.begin());
            }
            float whole = std::round(FPSavg);
            temp.value = static_cast<s16>(std::lround(FPSavg));
            if (FPSavg < whole + 0.04 && FPSavg > whole - 0.05) {
                temp.zero_rounded = true;
            }
            readings.push_back(temp);
        } else {
            readings.clear();
            readings.shrink_to_fit();
        }
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if ((keysHeld & HidNpadButton_StickL) && (keysHeld & HidNpadButton_StickR)) {
            EndFPSCounterThread();
            tsl::goBack();
            return true;
        } else if ((keysHeld & HidNpadButton_ZR) && (keysHeld & HidNpadButton_R)) {
            if ((keysHeld & HidNpadButton_Up) && base_y != 0) {
                base_y = 0;
            } else if ((keysHeld & HidNpadButton_Down) && base_y != 648) {
                base_y = 648;
            }
        }
        return false;
    }
};

//Full mode
class FullOverlay : public tsl::Gui {
public:
    FullOverlay() { }

    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, APP_VERSION);

        auto Status = new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
            //Print strings
            ///CPU
            if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck)) {
                renderer->drawString("CPUUsageFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 120, 20, renderer->a(0xFFFF));
                renderer->drawString(CPU_Hz_c, false, 20, 155, 15, renderer->a(0xFFFF));
                renderer->drawString(CPU_compressed_c, false, 20, 185, 15, renderer->a(0xFFFF));
            }

            ///GPU
            if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck) || R_SUCCEEDED(nvCheck)) {
                renderer->drawString("GPUUsageFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 285, 20, renderer->a(0xFFFF));
                if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck)) renderer->drawString(GPU_Hz_c, false, 20, 320, 15, renderer->a(0xFFFF));
                if (R_SUCCEEDED(nvCheck)) renderer->drawString(GPU_Load_c, false, 20, 335, 15, renderer->a(0xFFFF));
            }

            ///RAM
            if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck) || R_SUCCEEDED(Hinted)) {
                renderer->drawString("MEMUsageFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 375, 20, renderer->a(0xFFFF));
                if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck)) renderer->drawString(RAM_Hz_c, false, 20, 410, 15, renderer->a(0xFFFF));
                if (R_SUCCEEDED(Hinted)) {
                    renderer->drawString(RAM_compressed_c, false, 20, 440, 15, renderer->a(0xFFFF));
                    renderer->drawString(RAM_var_compressed_c, false, 110, 440, 15, renderer->a(0xFFFF));
                }
            }

            ///Thermal
            if (R_SUCCEEDED(tsCheck) || R_SUCCEEDED(tcCheck) || R_SUCCEEDED(fanCheck)) {
                renderer->drawString("ThermalUsageFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 540, 20, renderer->a(0xFFFF));
                if (R_SUCCEEDED(tsCheck)) renderer->drawString(BatteryDraw_c, false, 20, 575, 15, renderer->a(0xFFFF));
                if (R_SUCCEEDED(tsCheck)) renderer->drawString("TemperaturesFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 590, 15, renderer->a(0xFFFF));
                if (R_SUCCEEDED(tsCheck)) renderer->drawString(SoCPCB_temperature_c, false, 110, 590, 15, renderer->a(0xFFFF));
                if (R_SUCCEEDED(fanCheck)) renderer->drawString(Rotation_SpeedLevel_c, false, 20, 635, 15, renderer->a(0xFFFF));
            }

            ///FPS
            if (GameRunning) {
                renderer->drawString(FPS_compressed_c, false, 215, 120, 20, renderer->a(0xFFFF));
                renderer->drawString(FPS_var_compressed_c, false, 315, 120, 20, renderer->a(0xFFFF));
            }

            if (refreshrate == 5) renderer->drawString("DecreaseRefreshRateHitFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 675, 15, renderer->a(0xFFFF));
            else if (refreshrate == 1) renderer->drawString("IncreaseRefreshRateHitFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 675, 15, renderer->a(0xFFFF));
        });

        rootFrame->setContent(Status);

        return rootFrame;
    }

    virtual void update() override {
        if (TeslaFPS == 60) TeslaFPS = 1;
        //In case of getting more than systemtickfrequency in idle, make it equal to systemtickfrequency to get 0% as output and nothing less
        //This is because making each loop also takes time, which is not considered because this will take also additional time
        if (idletick0 > systemtickfrequency) idletick0 = systemtickfrequency;
        if (idletick1 > systemtickfrequency) idletick1 = systemtickfrequency;
        if (idletick2 > systemtickfrequency) idletick2 = systemtickfrequency;
        if (idletick3 > systemtickfrequency) idletick3 = systemtickfrequency;

        //Make stuff ready to print
        ///CPU
        snprintf(CPU_Hz_c, sizeof CPU_Hz_c, "UpdateCPUFreqFullOverlayCustomDrawerText"_tr.c_str(), (float)CPU_Hz / 1000000);
        snprintf(CPU_Usage0, sizeof CPU_Usage0, "UpdateCPUCore0FullOverlayCustomDrawerText"_tr.c_str(), ((double)systemtickfrequency - (double)idletick0) / (double)systemtickfrequency * 100, "%");
        snprintf(CPU_Usage1, sizeof CPU_Usage1, "UpdateCPUCore1FullOverlayCustomDrawerText"_tr.c_str(), ((double)systemtickfrequency - (double)idletick1) / (double)systemtickfrequency * 100, "%");
        snprintf(CPU_Usage2, sizeof CPU_Usage2, "UpdateCPUCore2FullOverlayCustomDrawerText"_tr.c_str(), ((double)systemtickfrequency - (double)idletick2) / (double)systemtickfrequency * 100, "%");
        snprintf(CPU_Usage3, sizeof CPU_Usage3, "UpdateCPUCore3FullOverlayCustomDrawerText"_tr.c_str(), ((double)systemtickfrequency - (double)idletick3) / (double)systemtickfrequency * 100, "%");
        snprintf(CPU_compressed_c, sizeof CPU_compressed_c, "%s\n%s\n%s\n%s", CPU_Usage0, CPU_Usage1, CPU_Usage2, CPU_Usage3);

        ///GPU
        snprintf(GPU_Hz_c, sizeof GPU_Hz_c, "UpdateGPUFreqFullOverlayCustomDrawerText"_tr.c_str(), (float)GPU_Hz / 1000000);
        snprintf(GPU_Load_c, sizeof GPU_Load_c, "UpdateGPULoadFullOverlayCustomDrawerText"_tr.c_str(), (float)GPU_Load_u / 10, "%");

        ///RAM
        snprintf(RAM_Hz_c, sizeof RAM_Hz_c, "UpdateMEMFreqFullOverlayCustomDrawerText"_tr.c_str(), (float)RAM_Hz / 1000000);
        float RAM_Total_application_f = (float)RAM_Total_application_u / 1024 / 1024;
        float RAM_Total_applet_f = (float)RAM_Total_applet_u / 1024 / 1024;
        float RAM_Total_system_f = (float)RAM_Total_system_u / 1024 / 1024;
        float RAM_Total_systemunsafe_f = (float)RAM_Total_systemunsafe_u / 1024 / 1024;
        float RAM_Total_all_f = RAM_Total_application_f + RAM_Total_applet_f + RAM_Total_system_f + RAM_Total_systemunsafe_f;
        float RAM_Used_application_f = (float)RAM_Used_application_u / 1024 / 1024;
        float RAM_Used_applet_f = (float)RAM_Used_applet_u / 1024 / 1024;
        float RAM_Used_system_f = (float)RAM_Used_system_u / 1024 / 1024;
        float RAM_Used_systemunsafe_f = (float)RAM_Used_systemunsafe_u / 1024 / 1024;
        float RAM_Used_all_f = RAM_Used_application_f + RAM_Used_applet_f + RAM_Used_system_f + RAM_Used_systemunsafe_f;
        snprintf(RAM_all_c, sizeof RAM_all_c, "UpdateMEMSizeFullOverlayCustomDrawerText"_tr.c_str());
        snprintf(RAM_application_c, sizeof RAM_application_c, "UpdateMEMAppFullOverlayCustomDrawerText"_tr.c_str());
        snprintf(RAM_applet_c, sizeof RAM_applet_c, "UpdateMEMAppletFullOverlayCustomDrawerText"_tr.c_str());
        snprintf(RAM_system_c, sizeof RAM_system_c, "UpdateMEMSystemFullOverlayCustomDrawerText"_tr.c_str());
        snprintf(RAM_systemunsafe_c, sizeof RAM_systemunsafe_c, "UpdateMEMSystemReservedFullOverlayCustomDrawerText"_tr.c_str());
        snprintf(RAM_compressed_c, sizeof RAM_compressed_c, "%s\n%s\n%s\n%s\n%s", RAM_all_c, RAM_application_c, RAM_applet_c, RAM_system_c, RAM_systemunsafe_c);
        snprintf(RAM_all_c, sizeof RAM_all_c, "%4.2f / %4.2f MB", RAM_Used_all_f, RAM_Total_all_f);
        snprintf(RAM_application_c, sizeof RAM_application_c, "%4.2f / %4.2f MB", RAM_Used_application_f, RAM_Total_application_f);
        snprintf(RAM_applet_c, sizeof RAM_applet_c, "%4.2f / %4.2f MB", RAM_Used_applet_f, RAM_Total_applet_f);
        snprintf(RAM_system_c, sizeof RAM_system_c, "%4.2f / %4.2f MB", RAM_Used_system_f, RAM_Total_system_f);
        snprintf(RAM_systemunsafe_c, sizeof RAM_systemunsafe_c, "%4.2f / %4.2f MB", RAM_Used_systemunsafe_f, RAM_Total_systemunsafe_f);
        snprintf(RAM_var_compressed_c, sizeof RAM_var_compressed_c, "%s\n%s\n%s\n%s\n%s", RAM_all_c, RAM_application_c, RAM_applet_c, RAM_system_c, RAM_systemunsafe_c);

        ///Thermal
        snprintf(BatteryDraw_c, sizeof BatteryDraw_c, "UpdateBatteryPowerFlowFullOverlayCustomDrawerText"_tr.c_str(), PowerConsumption);
        if (hosversionAtLeast(14,0,0))
            snprintf(SoCPCB_temperature_c, sizeof SoCPCB_temperature_c, "UpdateThermalHos14FullOverlayCustomDrawerText"_tr.c_str(), SOC_temperatureC, PCB_temperatureC, (float)skin_temperaturemiliC / 1000);
        else
            snprintf(SoCPCB_temperature_c, sizeof SoCPCB_temperature_c, "UpdateThermalFullOverlayCustomDrawerText"_tr.c_str(), (float)SOC_temperatureC / 1000, (float)PCB_temperatureC / 1000, (float)skin_temperaturemiliC / 1000);
        snprintf(Rotation_SpeedLevel_c, sizeof Rotation_SpeedLevel_c, "UpdateFanRotationLevelFullOverlayCustomDrawerText"_tr.c_str(), Rotation_SpeedLevel_f * 100, "%");

        ///FPS
        snprintf(FPS_c, sizeof FPS_c, "UpdatePFPSFullOverlayCustomDrawerText"_tr.c_str()); //Pushed Frames Per Second
        snprintf(FPSavg_c, sizeof FPSavg_c, "UpdateFPSFullOverlayCustomDrawerText"_tr.c_str()); //Frames Per Second calculated from averaged frametime
        snprintf(FPS_compressed_c, sizeof FPS_compressed_c, "%s\n%s", FPS_c, FPSavg_c);
        snprintf(FPS_var_compressed_c, sizeof FPS_var_compressed_c, "%u\n%2.2f", FPS, FPSavg);

    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if ((keysHeld & HidNpadButton_StickL) && (keysHeld & HidNpadButton_StickR)) {
            CloseThreads();
            tsl::goBack();
            return true;
        }
        return false;
    }
};

//Mini mode
class MiniOverlay : public tsl::Gui {
public:
    MiniOverlay() { }

    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("", "");

        auto Status = new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
            if (!GameRunning) renderer->drawRect(0, 0, tsl::cfg::FramebufferWidth - 150, 95, a(0x7111));
            else renderer->drawRect(0, 0, tsl::cfg::FramebufferWidth - 150, 125, a(0x7111));

            //Print strings
            ///CPU
            if (GameRunning) renderer->drawString("GameRunningMiniOverlayCustomDrawerText"_tr.c_str(), false, 0, 15, 15, renderer->a(0xFFFF));
            else renderer->drawString("NoGameRunningMiniOverlayCustomDrawerText"_tr.c_str(), false, 0, 15, 15, renderer->a(0xFFFF));

            ///GPU
            renderer->drawString(Variables, false, 70, 15, 15, renderer->a(0xFFFF));
        });

        rootFrame->setContent(Status);

        return rootFrame;
    }

    virtual void update() override {
        if (TeslaFPS == 60) TeslaFPS = 1;
        //In case of getting more than systemtickfrequency in idle, make it equal to systemtickfrequency to get 0% as output and nothing less
        //This is because making each loop also takes time, which is not considered because this will take also additional time
        if (idletick0 > systemtickfrequency) idletick0 = systemtickfrequency;
        if (idletick1 > systemtickfrequency) idletick1 = systemtickfrequency;
        if (idletick2 > systemtickfrequency) idletick2 = systemtickfrequency;
        if (idletick3 > systemtickfrequency) idletick3 = systemtickfrequency;

        //Make stuff ready to print
        ///CPU
        double percent = ((double)systemtickfrequency - (double)idletick0) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage0, sizeof CPU_Usage0, "%.0f%s", percent, "%");
        percent = ((double)systemtickfrequency - (double)idletick1) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage1, sizeof CPU_Usage1, "%.0f%s", percent, "%");
        percent = ((double)systemtickfrequency - (double)idletick2) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage2, sizeof CPU_Usage2, "%.0f%s", percent, "%");
        percent = ((double)systemtickfrequency - (double)idletick3) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage3, sizeof CPU_Usage3, "%.0f%s", percent, "%");
        snprintf(CPU_compressed_c, sizeof CPU_compressed_c, "[%s,%s,%s,%s]@%.1f", CPU_Usage0, CPU_Usage1, CPU_Usage2, CPU_Usage3, (float)CPU_Hz / 1000000);

        ///GPU
        snprintf(GPU_Load_c, sizeof GPU_Load_c, "%.1f%s@%.1f", (float)GPU_Load_u / 10, "%", (float)GPU_Hz / 1000000);

        ///RAM
        float RAM_Total_application_f = (float)RAM_Total_application_u / 1024 / 1024;
        float RAM_Total_applet_f = (float)RAM_Total_applet_u / 1024 / 1024;
        float RAM_Total_system_f = (float)RAM_Total_system_u / 1024 / 1024;
        float RAM_Total_systemunsafe_f = (float)RAM_Total_systemunsafe_u / 1024 / 1024;
        float RAM_Total_all_f = RAM_Total_application_f + RAM_Total_applet_f + RAM_Total_system_f + RAM_Total_systemunsafe_f;
        float RAM_Used_application_f = (float)RAM_Used_application_u / 1024 / 1024;
        float RAM_Used_applet_f = (float)RAM_Used_applet_u / 1024 / 1024;
        float RAM_Used_system_f = (float)RAM_Used_system_u / 1024 / 1024;
        float RAM_Used_systemunsafe_f = (float)RAM_Used_systemunsafe_u / 1024 / 1024;
        float RAM_Used_all_f = RAM_Used_application_f + RAM_Used_applet_f + RAM_Used_system_f + RAM_Used_systemunsafe_f;
        snprintf(RAM_all_c, sizeof RAM_all_c, "%.0f/%.0fMB", RAM_Used_all_f, RAM_Total_all_f);
        snprintf(RAM_var_compressed_c, sizeof RAM_var_compressed_c, "%s@%.1f", RAM_all_c, (float)RAM_Hz / 1000000);

        // Thermal
        snprintf(SoCPCB_temperature_c, sizeof SoCPCB_temperature_c, "%0.2fW", PowerConsumption);
        if (hosversionAtLeast(14,0,0))
            snprintf(skin_temperature_c, sizeof skin_temperature_c, "%2d\u00B0C/%2d\u00B0C/%2.1f\u00B0C", SOC_temperatureC, PCB_temperatureC, (float)skin_temperaturemiliC / 1000);
        else
            snprintf(skin_temperature_c, sizeof skin_temperature_c, "%2.1f\u00B0C/%2.1f\u00B0C/%2.1f\u00B0C", (float)SOC_temperatureC / 1000, (float)PCB_temperatureC / 1000, (float)skin_temperaturemiliC / 1000);
        snprintf(Rotation_SpeedLevel_c, sizeof Rotation_SpeedLevel_c, "%2.2f%s", Rotation_SpeedLevel_f * 100, "%");

        ///FPS
        snprintf(FPS_c, sizeof FPS_c, "UpdateFPSMiniOverlayCustomDrawerText"_tr.c_str()); //Pushed Frames Per Second
        snprintf(FPSavg_c, sizeof FPSavg_c, "UpdateFPSAvgMiniOverlayCustomDrawerText"_tr.c_str()); //Frames Per Second calculated from averaged frametime
        snprintf(FPS_compressed_c, sizeof FPS_compressed_c, "%s\n%s", FPS_c, FPSavg_c);
        snprintf(FPS_var_compressed_c, sizeof FPS_compressed_c, "%u\n%2.2f", FPS, FPSavg);

        if (GameRunning) snprintf(Variables, sizeof Variables, "%s\n%s\n%s\n%s\n%s\n%s\n%s", CPU_compressed_c, GPU_Load_c, RAM_var_compressed_c, skin_temperature_c, Rotation_SpeedLevel_c, SoCPCB_temperature_c, FPS_var_compressed_c);
        else snprintf(Variables, sizeof Variables, "%s\n%s\n%s\n%s\n%s\n%s", CPU_compressed_c, GPU_Load_c, RAM_var_compressed_c, skin_temperature_c, Rotation_SpeedLevel_c, SoCPCB_temperature_c);
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if ((keysHeld & HidNpadButton_StickL) && (keysHeld & HidNpadButton_StickR)) {
            CloseThreads();
            tsl::goBack();
            return true;
        }
        return false;
    }
};

//Mini mode
class MicroOverlay : public tsl::Gui {
public:
    MicroOverlay() {}

    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("", "");

        auto Status = new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
            if (!GameRunning) {
                uint32_t size = 18;
                uint32_t offset1 = 0;
                uint32_t offset2 = offset1 + 335;
                uint32_t offset3 = offset2 + 210;
                uint32_t offset4 = offset3 + 275;
                uint32_t offset5 = offset4 + 320;
                renderer->drawRect(0, 0, tsl::cfg::FramebufferWidth, 22, a(0x7111));
                renderer->drawString("NoGameRunningCPUMicroOverlayCustomDrawerText"_tr.c_str(), false, offset1, size, size, renderer->a(0xFCCF));
                renderer->drawString("NoGameRunningGPUMicroOverlayCustomDrawerText"_tr.c_str(), false, offset2, size, size, renderer->a(0xFCCF));
                renderer->drawString("NoGameRunningRAMMicroOverlayCustomDrawerText"_tr.c_str(), false, offset3, size, size, renderer->a(0xFCCF));
                renderer->drawString("NoGameRunningBRDMicroOverlayCustomDrawerText"_tr.c_str(), false, offset4, size, size, renderer->a(0xFCCF));
                renderer->drawString("NoGameRunningFANMicroOverlayCustomDrawerText"_tr.c_str(), false, offset5, size, size, renderer->a(0xFCCF));
                renderer->drawString(CPU_compressed_c, false, offset1+42, size, size, renderer->a(0xFFFF));
                renderer->drawString(GPU_Load_c, false, offset2+45, size, size, renderer->a(0xFFFF));
                renderer->drawString(RAM_var_compressed_c, false, offset3+47, size, size, renderer->a(0xFFFF));
                renderer->drawString(skin_temperature_c, false, offset4+45, size, size, renderer->a(0xFFFF));
                renderer->drawString(Rotation_SpeedLevel_c, false, offset5+43, size, size, renderer->a(0xFFFF));
            } else {
                uint32_t size = 18;
                uint32_t offset1 = 0;
                uint32_t offset2 = offset1 + 335;
                uint32_t offset3 = offset2 + 200;
                uint32_t offset4 = offset3 + 265;
                uint32_t offset5 = offset4 + 245;
                uint32_t offset6 = offset5 + 130;
                renderer->drawRect(0, 0, tsl::cfg::FramebufferWidth, 22, a(0x7111));
                renderer->drawString("GameRunningCPUMicroOverlayCustomDrawerText"_tr.c_str(), false, offset1, size, size, renderer->a(0xFCCF));
                renderer->drawString("GameRunningGPUMicroOverlayCustomDrawerText"_tr.c_str(), false, offset2, size, size, renderer->a(0xFCCF));
                renderer->drawString("GameRunningRAMMicroOverlayCustomDrawerText"_tr.c_str(), false, offset3, size, size, renderer->a(0xFCCF));
                renderer->drawString("GameRunningBRDMicroOverlayCustomDrawerText"_tr.c_str(), false, offset4, size, size, renderer->a(0xFCCF));
                renderer->drawString("GameRunningFANMicroOverlayCustomDrawerText"_tr.c_str(), false, offset5, size, size, renderer->a(0xFCCF));
                renderer->drawString("GameRunningFPSMicroOverlayCustomDrawerText"_tr.c_str(), false, offset6, size, size, renderer->a(0xFCCF));
                renderer->drawString(CPU_compressed_c, false, offset1+42, size, size, renderer->a(0xFFFF));
                renderer->drawString(GPU_Load_c, false, offset2+45, size, size, renderer->a(0xFFFF));
                renderer->drawString(RAM_var_compressed_c, false, offset3+47, size, size, renderer->a(0xFFFF));
                renderer->drawString(skin_temperature_c, false, offset4+45, size, size, renderer->a(0xFFFF));
                renderer->drawString(Rotation_SpeedLevel_c, false, offset5+43, size, size, renderer->a(0xFFFF));
                renderer->drawString(FPS_var_compressed_c, false, offset6+40, size, size, renderer->a(0xFFFF));
            }
        });

        rootFrame->setContent(Status);

        return rootFrame;
    }

    virtual void update() override {
        if (TeslaFPS == 60) {
            TeslaFPS = 1;
            tsl::hlp::requestForeground(false);
        }
        //In case of getting more than systemtickfrequency in idle, make it equal to systemtickfrequency to get 0% as output and nothing less
        //This is because making each loop also takes time, which is not considered because this will take also additional time
        if (idletick0 > systemtickfrequency) idletick0 = systemtickfrequency;
        if (idletick1 > systemtickfrequency) idletick1 = systemtickfrequency;
        if (idletick2 > systemtickfrequency) idletick2 = systemtickfrequency;
        if (idletick3 > systemtickfrequency) idletick3 = systemtickfrequency;

        //Make stuff ready to print
        ///CPU
        double percent = ((double)systemtickfrequency - (double)idletick0) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage0, sizeof CPU_Usage0, "%.0f%s", percent, "%");
        percent = ((double)systemtickfrequency - (double)idletick1) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage1, sizeof CPU_Usage1, "%.0f%s", percent, "%");
        percent = ((double)systemtickfrequency - (double)idletick2) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage2, sizeof CPU_Usage2, "%.0f%s", percent, "%");
        percent = ((double)systemtickfrequency - (double)idletick3) / (double)systemtickfrequency * 100;
        snprintf(CPU_Usage3, sizeof CPU_Usage3, "%.0f%s", percent, "%");
        snprintf(CPU_compressed_c, sizeof CPU_compressed_c, "[%s,%s,%s,%s]@%.1f", CPU_Usage0, CPU_Usage1, CPU_Usage2, CPU_Usage3, (float)CPU_Hz / 1000000);

        ///GPU
        snprintf(GPU_Load_c, sizeof GPU_Load_c, "%.1f%s@%.1f", (float)GPU_Load_u / 10, "%", (float)GPU_Hz / 1000000);

        ///RAM
        float RAM_Total_application_f = (float)RAM_Total_application_u / 1024 / 1024;
        float RAM_Total_applet_f = (float)RAM_Total_applet_u / 1024 / 1024;
        float RAM_Total_system_f = (float)RAM_Total_system_u / 1024 / 1024;
        float RAM_Total_systemunsafe_f = (float)RAM_Total_systemunsafe_u / 1024 / 1024;
        float RAM_Total_all_f = RAM_Total_application_f + RAM_Total_applet_f + RAM_Total_system_f + RAM_Total_systemunsafe_f;
        float RAM_Used_application_f = (float)RAM_Used_application_u / 1024 / 1024;
        float RAM_Used_applet_f = (float)RAM_Used_applet_u / 1024 / 1024;
        float RAM_Used_system_f = (float)RAM_Used_system_u / 1024 / 1024;
        float RAM_Used_systemunsafe_f = (float)RAM_Used_systemunsafe_u / 1024 / 1024;
        float RAM_Used_all_f = RAM_Used_application_f + RAM_Used_applet_f + RAM_Used_system_f + RAM_Used_systemunsafe_f;
        snprintf(RAM_all_c, sizeof RAM_all_c, "%.0f/%.0fMB", RAM_Used_all_f, RAM_Total_all_f);
        snprintf(RAM_var_compressed_c, sizeof RAM_var_compressed_c, "%s@%.1f", RAM_all_c, (float)RAM_Hz / 1000000);

        ///Thermal
        if (GameRunning) {
            if (hosversionAtLeast(14,0,0))
                snprintf(skin_temperature_c, sizeof skin_temperature_c, "%2d/%2d/%2.0f\u00B0C@%+.2fW", SOC_temperatureC, PCB_temperatureC, (float)skin_temperaturemiliC / 1000, PowerConsumption);
            else
                snprintf(skin_temperature_c, sizeof skin_temperature_c, "%2.0f/%2.0f/%2.0f\u00B0C@%+.2fW", (float)SOC_temperatureC / 1000, (float)PCB_temperatureC / 1000, (float)skin_temperaturemiliC / 1000, PowerConsumption);
        } else {
            if (hosversionAtLeast(14,0,0))
                snprintf(skin_temperature_c, sizeof skin_temperature_c, "%2d/%2d/%2.1f\u00B0C@%+.2fW", SOC_temperatureC, PCB_temperatureC, (float)skin_temperaturemiliC / 1000, PowerConsumption);
            else
                snprintf(skin_temperature_c, sizeof skin_temperature_c, "%2.1f/%2.1f/%2.1f\u00B0C@%+.2fW", (float)SOC_temperatureC / 1000, (float)PCB_temperatureC / 1000, (float)skin_temperaturemiliC / 1000, PowerConsumption);
        }
        snprintf(Rotation_SpeedLevel_c, sizeof Rotation_SpeedLevel_c, "%2.2f%s", Rotation_SpeedLevel_f * 100, "%");

        ///FPS
        snprintf(FPS_var_compressed_c, sizeof FPS_var_compressed_c, "%2.1f", FPSavg);

        //Debug
        /*
        snprintf(CPU_compressed_c, sizeof CPU_compressed_c, "[100%s,100%s,100%s,100%s]@1785.0", "%", "%", "%", "%");
        snprintf(GPU_Load_c, sizeof GPU_Load_c, "100.0%s@2400.0", "%");
        snprintf(RAM_var_compressed_c, sizeof RAM_var_compressed_c, "4444/4444MB@4444.4");
        if (GameRunning) {
            snprintf(skin_temperature_c, sizeof skin_temperature_c, "88/88/88\u00B0C@15.55W");
        }
        else snprintf(skin_temperature_c, sizeof skin_temperature_c, "88.8/88.8/88.8\u00B0C@15.55W");
        snprintf(Rotation_SpeedLevel_c, sizeof Rotation_SpeedLevel_c, "100.00%s", "%");
        snprintf(FPS_var_compressed_c, sizeof FPS_var_compressed_c, "60.0");
        */
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if ((keysHeld & HidNpadButton_StickL) && (keysHeld & HidNpadButton_StickR)) {
            TeslaFPS = 60;
            refreshrate = 60;
            tsl::setNextOverlay(filepath);
            tsl::Overlay::get()->close();
            return true;
        }
        return false;
    }
};

//Battery
class BatteryOverlay : public tsl::Gui {
public:
    BatteryOverlay() { }

    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, APP_VERSION);

        auto Status = new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
            if (R_SUCCEEDED(psmCheck)) {
                renderer->drawString("BatteryOrChargerUsageBatteryOverlayCustomDrawerText"_tr.c_str(), false, 20, 120, 20, renderer->a(0xFFFF));
                renderer->drawString(Battery_c, false, 20, 155, 18, renderer->a(0xFFFF));
            }

        });

        rootFrame->setContent(Status);

        return rootFrame;
    }

    virtual void update() override {
        ///Battery
        if (_batteryChargeInfoFields.ChargerType) {
            const char *chargerType = "";
            switch (_batteryChargeInfoFields.ChargerType) {
            case PD:
                chargerType = "PD";
                break;
            case TypeC_1500mA:
                chargerType = "TypeC_1500mA";
                break;
            case TypeC_3000mA:
                chargerType = "TypeC_3000mA";
                break;
            case DCP:
                chargerType = "DCP";
                break;
            case CDP:
                chargerType = "CDP";
                break;
            case SDP:
                chargerType = "SDP";
                break;
            case Apple_500mA:
                chargerType = "Apple_500mA";
                break;
            case Apple_1000mA:
                chargerType = "Apple_1000mA";
                break;
            case Apple_2000mA:
                chargerType = "Apple_2000mA";
                break;
            case None:
            default:
                chargerType = "None";
                break;
            }
            snprintf(Battery_c, sizeof Battery_c,
                "UpdateBetteryChargeInfoChargerBatteryOverlayCustomDrawerText"_tr.c_str(),
                (float)_batteryChargeInfoFields.BatteryTemperature / 1000,
                (float)_batteryChargeInfoFields.RawBatteryCharge / 1000, "%",
                (float)_batteryChargeInfoFields.BatteryAge / 1000, "%",
                batVoltageAvg,
                batCurrentAvg,
                PowerConsumption,
                chargerType,
                _batteryChargeInfoFields.ChargerVoltageLimit,
                _batteryChargeInfoFields.ChargerCurrentLimit
            );
        } else {
            snprintf(Battery_c, sizeof Battery_c,
                "UpdateBetteryChargeInfoBetteryBatteryOverlayCustomDrawerText"_tr.c_str(),
                (float)_batteryChargeInfoFields.BatteryTemperature / 1000,
                (float)_batteryChargeInfoFields.RawBatteryCharge / 1000, "%",
                (float)_batteryChargeInfoFields.BatteryAge / 1000, "%",
                batVoltageAvg,
                batCurrentAvg,
                PowerConsumption
            );
        }
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if (keysHeld & HidNpadButton_B) {
            CloseThreads();
            svcSleepThread(500'000'000);
            tsl::goBack();
            return true;
        }
        return false;
    }
};

void StartMiscThread() {
    threadCreate(&t0, Misc2, NULL, NULL, 0x1000, 0x3F, 3);
    threadStart(&t0);
}

void EndMiscThread() {
    threadexit = true;
    threadWaitForExit(&t0);
    threadClose(&t0);
    threadexit = false;
}

class MiscOverlay : public tsl::Gui {
public:
    MiscOverlay() { }

    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, APP_VERSION);

        auto Status = new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
            ///DSP
            if (R_SUCCEEDED(audsnoopCheck)) {
                renderer->drawString(DSP_Load_c, false, 20, 120, 20, renderer->a(0xFFFF));
            }

            //NVDEC
            if (R_SUCCEEDED(nvdecCheck)) {
                renderer->drawString(NVDEC_Hz_c, false, 20, 165, 20, renderer->a(0xFFFF));
            }

            if (R_SUCCEEDED(nifmCheck)) {
                renderer->drawString("NetworkMiscOverlayCustomDrawerText"_tr.c_str(), false, 20, 210, 20, renderer->a(0xFFFF));
                if (!Nifm_internet_rc) {
                    if (NifmConnectionType == NifmInternetConnectionType_WiFi) {
                        renderer->drawString("NetworkTypeWifiMiscOverlayCustomDrawerText"_tr.c_str(), false, 20, 235, 18, renderer->a(0xFFFF));
                        if (!Nifm_profile_rc) {
                            if (Nifm_showpass)
                                renderer->drawString(Nifm_pass, false, 20, 260, 15, renderer->a(0xFFFF));
                            else
                                renderer->drawString("NetworkTypeWifiShowPasswordMiscOverlayCustomDrawerText"_tr.c_str(), false, 20, 260, 15, renderer->a(0xFFFF));
                        }
                    } else if (NifmConnectionType == NifmInternetConnectionType_Ethernet)
                        renderer->drawString("NetworkTypeEthernetMiscOverlayCustomDrawerText"_tr.c_str(), false, 20, 235, 18, renderer->a(0xFFFF));
                } else
                    renderer->drawString("NetworkTypeNotConnectedMiscOverlayCustomDrawerText"_tr.c_str(), false, 20, 235, 18, renderer->a(0xFFFF));
            }
        });

        rootFrame->setContent(Status);

        return rootFrame;
    }

    virtual void update() override {
        snprintf(DSP_Load_c, sizeof DSP_Load_c, "UpdateDSPUsageMiscOverlayCustomDrawerText"_tr.c_str(), DSP_Load_u);
        snprintf(NVDEC_Hz_c, sizeof NVDEC_Hz_c, "UpdateNVDECClockRateMiscOverlayCustomDrawerText"_tr.c_str(), (float)NVDEC_Hz / 1000);
        char pass_temp1[25] = "";
        char pass_temp2[25] = "";
        char pass_temp3[17] = "";
        if (Nifm_profile.wireless_setting_data.passphrase_len > 48) {
            memcpy(&pass_temp1, &(Nifm_profile.wireless_setting_data.passphrase[0]), 24);
            memcpy(&pass_temp2, &(Nifm_profile.wireless_setting_data.passphrase[24]), 24);
            memcpy(&pass_temp3, &(Nifm_profile.wireless_setting_data.passphrase[48]), 16);
        } else if (Nifm_profile.wireless_setting_data.passphrase_len > 24) {
            memcpy(&pass_temp1, &(Nifm_profile.wireless_setting_data.passphrase[0]), 24);
            memcpy(&pass_temp2, &(Nifm_profile.wireless_setting_data.passphrase[24]), 24);
        } else {
            memcpy(&pass_temp1, &(Nifm_profile.wireless_setting_data.passphrase[0]), 24);
        }
        snprintf(Nifm_pass, sizeof Nifm_pass, "%s\n%s\n%s", pass_temp1, pass_temp2, pass_temp3);
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if (keysHeld & HidNpadButton_Y) {
            Nifm_showpass = true;
        } else Nifm_showpass = false;

        if (keysHeld & HidNpadButton_B) {
            EndMiscThread();
            nifmExit();
            svcSleepThread(500'000'000);
            tsl::goBack();
            return true;
        }
        return false;
    }
};

//Graphs
class GraphsMenu : public tsl::Gui {
public:
    GraphsMenu() { }

    virtual tsl::elm::Element* createUI() override {
        auto rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, "GraphsMenuListItemText"_tr);
        auto list = new tsl::elm::List();

        auto comFPSGraph = new tsl::elm::ListItem("FPSGraphsMenuListItemText"_tr);
        comFPSGraph->setClickListener([](uint64_t keys) {
            if (keys & HidNpadButton_A) {
                StartFPSCounterThread();
                TeslaFPS = 31;
                refreshrate = 31;
                IsFrameBackground = false;
                tsl::hlp::requestForeground(false);
                FullMode = false;
                tsl::changeTo<com_FPSGraph>();
                return true;
            }
            return false;
        });
        list->addItem(comFPSGraph);

        rootFrame->setContent(list);

        return rootFrame;
    }

    virtual void update() override {
        if (TeslaFPS != 60) {
            FullMode = true;
            tsl::hlp::requestForeground(true);
            TeslaFPS = 60;
            IsFrameBackground = true;
            refreshrate = 60;
            systemtickfrequency = 19200000;
        }
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if (keysHeld & HidNpadButton_B) {
            svcSleepThread(300'000'000);
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
        auto rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, "OtherMenuListItemText"_tr);
        auto list = new tsl::elm::List();

        auto Battery = new tsl::elm::ListItem("BatteryOrChargerOtherMenuListItemText"_tr);
        Battery->setClickListener([](uint64_t keys) {
            if (keys & HidNpadButton_A) {
                StartBatteryThread();
                tsl::changeTo<BatteryOverlay>();
                return true;
            }
            return false;
        });
        list->addItem(Battery);

        auto Misc = new tsl::elm::ListItem("MiscOtherMenuListItemText"_tr);
        Misc->setClickListener([](uint64_t keys) {
            if (keys & HidNpadButton_A) {
                smInitialize();
                nifmCheck = nifmInitialize(NifmServiceType_Admin);
                smExit();
                StartMiscThread();
                tsl::changeTo<MiscOverlay>();
                return true;
            }
            return false;
        });
        list->addItem(Misc);

        rootFrame->setContent(list);

        return rootFrame;
    }

    virtual void update() override {}

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if (keysHeld & HidNpadButton_B) {
            svcSleepThread(300'000'000);
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
        auto rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, APP_VERSION);
        auto list = new tsl::elm::List();

        auto Full = new tsl::elm::ListItem("FullMainMenuListItemText"_tr);
        Full->setClickListener([](uint64_t keys) {
            if (keys & HidNpadButton_A) {
                StartThreads();
                TeslaFPS = 1;
                refreshrate = 1;
                tsl::hlp::requestForeground(false);
                tsl::changeTo<FullOverlay>();
                return true;
            }
            return false;
        });
        list->addItem(Full);
        auto Mini = new tsl::elm::ListItem("MiniMainMenuListItemText"_tr);
        Mini->setClickListener([](uint64_t keys) {
            if (keys & HidNpadButton_A) {
                StartThreads();
                TeslaFPS = 1;
                refreshrate = 1;
                IsFrameBackground = false;
                tsl::hlp::requestForeground(false);
                FullMode = false;
                tsl::changeTo<MiniOverlay>();
                return true;
            }
            return false;
        });
        list->addItem(Mini);
        bool ovlExists = false;
        tsl::hlp::doWithSDCardHandle([&ovlExists]{
            bool fileExist = false;
            FILE* test = fopen(std::string(folderpath + filename).c_str(), "rb");
            if (test) {
                fclose(test);
                fileExist = true;
                filepath = folderpath + filename;
            } else {
                test = fopen(std::string(folderpath + APPTITLE ".ovl").c_str(), "rb");
                if (test) {
                    fclose(test);
                    fileExist = true;
                    filepath = folderpath + APPTITLE ".ovl";
                }
            }
            if (fileExist) {
                ovlExists = true;
            }
        });
        if (ovlExists) {
            auto Micro = new tsl::elm::ListItem("MicroMainMenuListItemText"_tr);
            Micro->setClickListener([](uint64_t keys) {
                if (keys & HidNpadButton_A) {
                    tsl::setNextOverlay(filepath, "--microOverlay");
                    tsl::Overlay::get()->close();
                    return true;
                }
                return false;
            });
            list->addItem(Micro);
        }
        if (SaltySD) {
            auto comFPS = new tsl::elm::ListItem("FPSMainMenuListItemText"_tr);
            comFPS->setClickListener([](uint64_t keys) {
                if (keys & HidNpadButton_A) {
                    StartFPSCounterThread();
                    TeslaFPS = 31;
                    refreshrate = 31;
                    IsFrameBackground = false;
                    tsl::hlp::requestForeground(false);
                    FullMode = false;
                    tsl::changeTo<com_FPS>();
                    return true;
                }
                return false;
            });
            list->addItem(comFPS);
            auto Graphs = new tsl::elm::ListItem("GraphsMainMenuListItemText"_tr);
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
        if (TeslaFPS != 60) {
            FullMode = true;
            tsl::hlp::requestForeground(true);
            TeslaFPS = 60;
            IsFrameBackground = true;
            refreshrate = 60;
            systemtickfrequency = 19200000;
        }
    }

    virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
        if (keysHeld & HidNpadButton_B) {
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
            if (hosversionAtLeast(8,0,0)) clkrstCheck = clkrstInitialize();
            else pcvCheck = pcvInitialize();

            tsCheck = tsInitialize();
            if (hosversionAtLeast(5,0,0)) tcCheck = tcInitialize();

            if (R_SUCCEEDED(fanInitialize())) {
                if (hosversionAtLeast(7,0,0)) fanCheck = fanOpenController(&g_ICon, 0x3D000001);
                else fanCheck = fanOpenController(&g_ICon, 1);
            }

            if (R_SUCCEEDED(nvInitialize())) nvCheck = nvOpen(&fd, "/dev/nvhost-ctrl-gpu");
            if (R_SUCCEEDED(nvMapInit())) nvdecCheck = nvChannelCreate(&nvdecChannel, "/dev/nvhost-nvdec");

            if (R_SUCCEEDED(audsnoopInitialize())) audsnoopCheck = audsnoopEnableDspUsageMeasurement();

            psmCheck = psmInitialize();
            if (R_SUCCEEDED(psmCheck)) {
                psmService = psmGetServiceSession();
                i2cInitialize();
            }

            SaltySD = CheckPort();

            if (SaltySD) {
                LoadSharedMemory();
            }
        });
        Hinted = envIsSyscallHinted(0x6F);
    }

    virtual void exitServices() override {
        CloseThreads();
        shmemClose(&_sharedmemory);
        //Exit services
        clkrstExit();
        pcvExit();
        tsExit();
        tcExit();
        fanControllerClose(&g_ICon);
        fanExit();
        nvChannelClose(&nvdecChannel);
        nvMapExit();
        nvClose(fd);
        nvExit();
        i2cExit();
        psmExit();
        if (R_SUCCEEDED(audsnoopCheck)) {
            audsnoopDisableDspUsageMeasurement();
        }
        audsnoopExit();
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
        tsl::hlp::doWithSmSession([this] {
            if (hosversionAtLeast(8,0,0)) clkrstCheck = clkrstInitialize();
            else pcvCheck = pcvInitialize();

            if (R_SUCCEEDED(nvInitialize())) nvCheck = nvOpen(&fd, "/dev/nvhost-ctrl-gpu");

            tsCheck = tsInitialize();
            if (hosversionAtLeast(5,0,0)) tcCheck = tcInitialize();

            if (R_SUCCEEDED(fanInitialize())) {
                if (hosversionAtLeast(7,0,0)) fanCheck = fanOpenController(&g_ICon, 0x3D000001);
                else fanCheck = fanOpenController(&g_ICon, 1);
            }

            psmCheck = psmInitialize();
            if (R_SUCCEEDED(psmCheck)) {
                psmService = psmGetServiceSession();
                i2cInitialize();
            }

            SaltySD = CheckPort();

            if (SaltySD) {
                LoadSharedMemory();
            }
        });
        Hinted = envIsSyscallHinted(0x6F);
    }

    virtual void exitServices() override {
        CloseThreads();
        shmemClose(&_sharedmemory);
        //Exit services
        clkrstExit();
        pcvExit();
        tsExit();
        tcExit();
        fanControllerClose(&g_ICon);
        fanExit();
        i2cExit();
        psmExit();
        nvClose(fd);
        nvExit();
    }

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        StartThreads();
        refreshrate = 1;
        return initially<MicroOverlay>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
    }
};

// This function gets called on startup to create a new Overlay object
int main(int argc, char **argv) {
    if (argc > 0) {
        filename = argv[0];
    }
    for (u8 arg = 0; arg < argc; arg++) {
        if (strcasecmp(argv[arg], "--microOverlay") == 0) {
            framebufferWidth = 1280;
            framebufferHeight = 28;
            FullMode = false;
            IsFrameBackground = false;
            FILE* test = fopen(std::string(folderpath + filename).c_str(), "rb");
            if (test) {
                fclose(test);
                filepath = folderpath + filename;
            } else {
                test = fopen(std::string(folderpath + APPTITLE ".ovl").c_str(), "rb");
                if (test) {
                    fclose(test);
                    filepath = folderpath + APPTITLE ".ovl";
                }
            }
            return tsl::loop<MicroMode>(argc, argv);
        }
    }
    return tsl::loop<MonitorOverlay>(argc, argv);
}