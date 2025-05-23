class FullOverlay : public tsl::Gui {
private:
	uint64_t mappedButtons = MapButtons(keyCombo); // map buttons
	char DeltaCPU_c[12] = "";
	char DeltaGPU_c[12] = "";
	char DeltaRAM_c[12] = "";
	char RealCPU_Hz_c[64] = "";
	char RealGPU_Hz_c[64] = "";
	char RealRAM_Hz_c[64] = "";
	char GPU_Load_c[32] = "";
	char Rotation_SpeedLevel_c[64] = "";
	char RAM_compressed_c[64] = "";
	char RAM_var_compressed_c[128] = "";
	char CPU_Hz_c[64] = "";
	char GPU_Hz_c[64] = "";
	char RAM_Hz_c[64] = "";
	char CPU_compressed_c[160] = "";
	char SoCPCB_temperature_c[64] = "";
	char skin_temperature_c[32] = "";
	char BatteryDraw_c[64] = "";
	char FPS_var_compressed_c[64] = "";
	char RAM_load_c[64] = "";
	char Resolutions_c[64] = "";

	uint8_t COMMON_MARGIN = 20;
	FullSettings settings;
	uint64_t systemtickfrequency_impl = systemtickfrequency;
	std::string formattedKeyCombo = keyCombo;
	std::string message = "Hold to Exit";
public:
    FullOverlay() {
		tsl::hlp::doWithSDCardHandle([this] {
			GetConfigSettings(&settings);
		});
		mutexInit(&mutex_BatteryChecker);
		mutexInit(&mutex_Misc);
		tsl::hlp::requestForeground(false);
		TeslaFPS = settings.refreshRate;
		systemtickfrequency_impl /= settings.refreshRate;
		if (settings.setPosRight) {
			tsl::gfx::Renderer::getRenderer().setLayerPos(1248, 0);
		}
		deactivateOriginalFooter = true;
		formatButtonCombination(formattedKeyCombo);
		message = "HoldRefreshRateHitFullOverlayCustomDrawerText"_tr.c_str() + formattedKeyCombo + "ExitRefreshRateHitFullOverlayCustomDrawerText"_tr.c_str();
		StartThreads();
	}
	~FullOverlay() {
		CloseThreads();
		FullMode = true;
		tsl::hlp::requestForeground(true);
		IsFrameBackground = true;
		if (settings.setPosRight) {
			tsl::gfx::Renderer::getRenderer().setLayerPos(0, 0);
		}
		deactivateOriginalFooter = false;
	}

	resolutionCalls m_resolutionRenderCalls[8] = {0};
	resolutionCalls m_resolutionViewportCalls[8] = {0};
	resolutionCalls m_resolutionOutput[8] = {0};
	uint8_t resolutionLookup = 0;

	virtual tsl::elm::Element* createUI() override {
		rootFrame = new tsl::elm::OverlayFrame("PluginName"_tr, APP_VERSION);

		auto Status = new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer *renderer, u16 x, u16 y, u16 w, u16 h) {
			//Print strings
			///CPU
			if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck)) {

				uint32_t height_offset = 155;
				if (realCPU_Hz && settings.showRealFreqs) {
					height_offset = 162;
				}
				renderer->drawString("CPUUsageFullOverlayCustomDrawerText"_tr.c_str(), false, COMMON_MARGIN, 120, 20, renderer->a(0xFFFF));
				if (settings.showTargetFreqs) {
					renderer->drawString(CPU_Hz_c, false, COMMON_MARGIN, height_offset, 15, renderer->a(0xFFFF));
				}
				if (realCPU_Hz && settings.showRealFreqs) {
					renderer->drawString(RealCPU_Hz_c, false, COMMON_MARGIN, height_offset - 15, 15, renderer->a(0xFFFF));
					if (settings.showDeltas && settings.showTargetFreqs) {
						renderer->drawString(DeltaCPU_c, false, COMMON_MARGIN + 170, height_offset - 7, 15, renderer->a(0xFFFF));
					}
					else if (settings.showDeltas && !settings.showTargetFreqs) {
						renderer->drawString(DeltaCPU_c, false, COMMON_MARGIN + 170, height_offset - 15, 15, renderer->a(0xFFFF));
					}
				}
				else if (realCPU_Hz && settings.showDeltas && (settings.showRealFreqs || settings.showTargetFreqs)) {
					renderer->drawString(DeltaCPU_c, false, COMMON_MARGIN + 170, height_offset, 15, renderer->a(0xFFFF));
				}
				renderer->drawString(CPU_compressed_c, false, COMMON_MARGIN, height_offset + 30, 15, renderer->a(0xFFFF));
			}

			///GPU
			if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck) || R_SUCCEEDED(nvCheck)) {

				uint32_t height_offset = 320;
				if (realGPU_Hz && settings.showRealFreqs) {
					height_offset = 327;
				}

				renderer->drawString("GPUUsageFullOverlayCustomDrawerText"_tr.c_str(), false, COMMON_MARGIN, 285, 20, renderer->a(0xFFFF));
				if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck)) {
					if (settings.showTargetFreqs) {
						renderer->drawString(GPU_Hz_c, false, COMMON_MARGIN, height_offset, 15, renderer->a(0xFFFF));
					}
					if (realCPU_Hz && settings.showRealFreqs) {
						renderer->drawString(RealGPU_Hz_c, false, COMMON_MARGIN, height_offset - 15, 15, renderer->a(0xFFFF));
						if (settings.showDeltas && settings.showTargetFreqs) {
							renderer->drawString(DeltaGPU_c, false, COMMON_MARGIN + 170, height_offset - 7, 15, renderer->a(0xFFFF));
						}
						else if (settings.showDeltas && !settings.showTargetFreqs) {
							renderer->drawString(DeltaGPU_c, false, COMMON_MARGIN + 170, height_offset - 15, 15, renderer->a(0xFFFF));
						}
					}
					else if (realGPU_Hz && settings.showDeltas && (settings.showRealFreqs || settings.showTargetFreqs)) {
						renderer->drawString(DeltaGPU_c, false, COMMON_MARGIN + 170, height_offset, 15, renderer->a(0xFFFF));
					}
				}
				if (R_SUCCEEDED(nvCheck)) {
					renderer->drawString(GPU_Load_c, false, COMMON_MARGIN, height_offset + 15, 15, renderer->a(0xFFFF));
				}
			}

			///RAM
			if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck) || R_SUCCEEDED(Hinted)) {

				uint32_t height_offset = 410;
				if (realRAM_Hz && settings.showRealFreqs) {
					height_offset += 7;
				}

				renderer->drawString("RAMUsageFullOverlayCustomDrawerText"_tr.c_str(), false, COMMON_MARGIN, 375, 20, renderer->a(0xFFFF));
				if (R_SUCCEEDED(clkrstCheck) || R_SUCCEEDED(pcvCheck)) {
					if (settings.showTargetFreqs) {
						renderer->drawString(RAM_Hz_c, false, COMMON_MARGIN, height_offset, 15, renderer->a(0xFFFF));
					}
					if (realRAM_Hz && settings.showRealFreqs) {
						renderer->drawString(RealRAM_Hz_c, false, COMMON_MARGIN, height_offset - 15, 15, renderer->a(0xFFFF));
						if (settings.showDeltas && settings.showTargetFreqs) {
							renderer->drawString(DeltaRAM_c, false, COMMON_MARGIN + 170, height_offset - 7, 15, renderer->a(0xFFFF));
						}
						else if (settings.showDeltas && !settings.showTargetFreqs) {
							renderer->drawString(DeltaRAM_c, false, COMMON_MARGIN + 170, height_offset - 15, 15, renderer->a(0xFFFF));
						}
					}
					else if (realRAM_Hz && settings.showDeltas && (settings.showRealFreqs || settings.showTargetFreqs)) {
						renderer->drawString(DeltaRAM_c, false, COMMON_MARGIN + 170, height_offset, 15, renderer->a(0xFFFF));
					}
					if (R_SUCCEEDED(sysclkCheck)) {
						renderer->drawString(RAM_load_c, false, COMMON_MARGIN, height_offset+15, 15, renderer->a(0xFFFF));
					}
				}
				if (R_SUCCEEDED(Hinted)) {
					static auto dimensions = renderer->drawString("RAMTotalUsageFullOverlayCustomDrawerText"_tr.c_str(), false, 0, height_offset + 40, 15, renderer->a(0x0000));
					renderer->drawString("RAMTotalUsageFullOverlayCustomDrawerText"_tr.c_str(), false, COMMON_MARGIN, height_offset + 40, 15, renderer->a(0xFFFF));
					renderer->drawString(RAM_var_compressed_c, false, COMMON_MARGIN + dimensions.first, height_offset + 40, 15, renderer->a(0xFFFF));
				}
			}

			///Thermal
			if (R_SUCCEEDED(i2cCheck) || R_SUCCEEDED(tcCheck) || R_SUCCEEDED(pwmCheck)) {
				renderer->drawString("ThermalUsageFullOverlayCustomDrawerText"_tr.c_str(), false, 20, 550, 20, renderer->a(0xFFFF));
				if (R_SUCCEEDED(i2cCheck)) renderer->drawString(BatteryDraw_c, false, COMMON_MARGIN, 575, 15, renderer->a(0xFFFF));
				if (R_SUCCEEDED(i2cCheck) || R_SUCCEEDED(tcCheck)) {
					static auto dimensions1 = renderer->drawString("ThermalTemperaturesDimensions1FullOverlayCustomDrawerText"_tr.c_str(), false, 0, 590, 15, renderer->a(0x0000));
					static auto dimensions2 = renderer->drawString("ThermalTemperaturesDimensions2FullOverlayCustomDrawerText"_tr.c_str(), false, 0, 590, 15, renderer->a(0x0000));
					renderer->drawString("ThermalTemperaturesFullOverlayCustomDrawerText"_tr.c_str(), false, COMMON_MARGIN, 590, 15, renderer->a(0xFFFF));
					renderer->drawString("ThermalTemperaturesItemsFullOverlayCustomDrawerText"_tr.c_str(), false, COMMON_MARGIN + dimensions1.first, 590, 15, renderer->a(0xFFFF));
					renderer->drawString(SoCPCB_temperature_c, false, COMMON_MARGIN + dimensions1.first + dimensions2.first, 590, 15, renderer->a(0xFFFF));
				}
				if (R_SUCCEEDED(pwmCheck)) renderer->drawString(Rotation_SpeedLevel_c, false, COMMON_MARGIN, 635, 15, renderer->a(0xFFFF));
			}

			///FPS
			if (GameRunning) {
				uint32_t width_offset = 170;
				if (settings.showFPS == true) {
					renderer->drawString(FPS_var_compressed_c, false, COMMON_MARGIN + width_offset, 120, 15, renderer->a(0xFCCF));
				}
				if ((settings.showRES == true) && (NxFps -> API) >= 1) {
					renderer->drawString(Resolutions_c, false, COMMON_MARGIN + width_offset, 215, 15, renderer->a(0xFCCF));
				}
			}

			renderer->drawString(message.c_str(), false, COMMON_MARGIN, 693, 23, renderer->a(0xFFFF));
		});

		rootFrame->setContent(Status);

		return rootFrame;
	}

	virtual void update() override {
		//Make stuff ready to print
		///CPU
		snprintf(CPU_compressed_c, sizeof(CPU_compressed_c), "UpdateCPUCoreFrequencyFullOverlayCustomDrawerText"_tr.c_str(),
			(idletick0 > systemtickfrequency_impl) ? 0.0f : (1.d - ((double)idletick0 / systemtickfrequency_impl)) * 100,
			(idletick1 > systemtickfrequency_impl) ? 0.0f : (1.d - ((double)idletick1 / systemtickfrequency_impl)) * 100,
			(idletick2 > systemtickfrequency_impl) ? 0.0f : (1.d - ((double)idletick2 / systemtickfrequency_impl)) * 100,
			(idletick3 > systemtickfrequency_impl) ? 0.0f : (1.d - ((double)idletick3 / systemtickfrequency_impl)) * 100);

		mutexLock(&mutex_Misc);
		snprintf(CPU_Hz_c, sizeof(CPU_Hz_c), "UpdateCPUTargetFrequencyFullOverlayCustomDrawerText"_tr.c_str(), CPU_Hz / 1000000, (CPU_Hz / 100000) % 10);
		if (realCPU_Hz) {
			snprintf(RealCPU_Hz_c, sizeof(RealCPU_Hz_c), "UpdateCPURealFrequencyFullOverlayCustomDrawerText"_tr.c_str(), realCPU_Hz / 1000000, (realCPU_Hz / 100000) % 10);
			int32_t deltaCPU = (int32_t)(realCPU_Hz / 1000) - (CPU_Hz / 1000);
			snprintf(DeltaCPU_c, sizeof(DeltaCPU_c), "Δ %d.%u", deltaCPU / 1000, abs(deltaCPU / 100) % 10);
		}

		///GPU
		snprintf(GPU_Hz_c, sizeof GPU_Hz_c, "UpdateCPUTargetFrequencyFullOverlayCustomDrawerText"_tr.c_str(), GPU_Hz / 1000000, (GPU_Hz / 100000) % 10);
		if (realGPU_Hz) {
			snprintf(RealGPU_Hz_c, sizeof(RealGPU_Hz_c), "UpdateGPURealFrequencyFullOverlayCustomDrawerText"_tr.c_str(), realGPU_Hz / 1000000, (realGPU_Hz / 100000) % 10);
			int32_t deltaGPU = (int32_t)(realGPU_Hz / 1000) - (GPU_Hz / 1000);
			snprintf(DeltaGPU_c, sizeof(DeltaGPU_c), "Δ %d.%u", deltaGPU / 1000, abs(deltaGPU / 100) % 10);
		}
		snprintf(GPU_Load_c, sizeof GPU_Load_c, "UpdateGPULoadFullOverlayCustomDrawerText"_tr.c_str(), GPU_Load_u / 10, GPU_Load_u % 10);

		///RAM
		snprintf(RAM_Hz_c, sizeof RAM_Hz_c, "UpdateCPUTargetFrequencyFullOverlayCustomDrawerText"_tr.c_str(), RAM_Hz / 1000000, (RAM_Hz / 100000) % 10);
		if (realRAM_Hz) {
			snprintf(RealRAM_Hz_c, sizeof(RealRAM_Hz_c), "UpdateRAMRealFrequencyFullOverlayCustomDrawerText"_tr.c_str(), realRAM_Hz / 1000000, (realRAM_Hz / 100000) % 10);
			int32_t deltaRAM = (int32_t)(realRAM_Hz / 1000) - (RAM_Hz / 1000);
			snprintf(DeltaRAM_c, sizeof(DeltaRAM_c), "Δ %d.%u", deltaRAM / 1000, abs(deltaRAM / 100) % 10);
		}
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

		snprintf(RAM_var_compressed_c, sizeof(RAM_var_compressed_c), "%4.2f / %4.2f MB\n%4.2f / %4.2f MB\n%4.2f / %4.2f MB\n%4.2f / %4.2f MB\n%4.2f / %4.2f MB", 
			RAM_Used_all_f, RAM_Total_all_f,
			RAM_Used_application_f, RAM_Total_application_f,
			RAM_Used_applet_f, RAM_Total_applet_f,
			RAM_Used_system_f, RAM_Total_system_f,
			RAM_Used_systemunsafe_f, RAM_Total_systemunsafe_f);

		if (R_SUCCEEDED(sysclkCheck)) {
			int RAM_GPU_Load = ramLoad[SysClkRamLoad_All] - ramLoad[SysClkRamLoad_Cpu];
			snprintf(RAM_load_c, sizeof RAM_load_c,
				"UpdateRAMLoadFullOverlayCustomDrawerText"_tr.c_str(),
				ramLoad[SysClkRamLoad_All] / 10, ramLoad[SysClkRamLoad_All] % 10,
				ramLoad[SysClkRamLoad_Cpu] / 10, ramLoad[SysClkRamLoad_Cpu] % 10,
				RAM_GPU_Load / 10, RAM_GPU_Load % 10);
		}
		///Thermal
		snprintf(SoCPCB_temperature_c, sizeof SoCPCB_temperature_c,
			"%2.1f\u00B0C\n%2.1f\u00B0C\n%2d.%d\u00B0C",
			SOC_temperatureF, PCB_temperatureF, skin_temperaturemiliC / 1000, (skin_temperaturemiliC / 100) % 10);
		snprintf(Rotation_SpeedLevel_c, sizeof Rotation_SpeedLevel_c, "UpdateFanRotationLevelFullOverlayCustomDrawerText"_tr.c_str(), Rotation_Duty);

		///FPS
		if (settings.showFPS == true && GameRunning) {
			snprintf(FPS_var_compressed_c, sizeof FPS_var_compressed_c, "UpdateFPSTitleFullOverlayCustomDrawerText"_tr.c_str(), FPS, FPSavg); 
		}

		//Resolutions
		if ((settings.showRES == true) && GameRunning && NxFps) {
			if (!resolutionLookup) {
				(NxFps -> renderCalls[0].calls) = 0xFFFF;
				resolutionLookup = 1;
			}
			else if (resolutionLookup == 1) {
				if ((NxFps -> renderCalls[0].calls) != 0xFFFF) resolutionLookup = 2;
			}
			else {
				memcpy(&m_resolutionRenderCalls, &(NxFps -> renderCalls), sizeof(m_resolutionRenderCalls));
				memcpy(&m_resolutionViewportCalls, &(NxFps -> viewportCalls), sizeof(m_resolutionViewportCalls));
				qsort(m_resolutionRenderCalls, 8, sizeof(resolutionCalls), compare);
				qsort(m_resolutionViewportCalls, 8, sizeof(resolutionCalls), compare);
				memset(&m_resolutionOutput, 0, sizeof(m_resolutionOutput));
				size_t out_iter = 0;
				bool found = false;
				for (size_t i = 0; i < 8; i++) {
					for (size_t x = 0; x < 8; x++) {
						if (m_resolutionRenderCalls[i].width == 0) {
							break;
						}
						if ((m_resolutionRenderCalls[i].width == m_resolutionViewportCalls[x].width) && (m_resolutionRenderCalls[i].height == m_resolutionViewportCalls[x].height)) {
							m_resolutionOutput[out_iter].width = m_resolutionRenderCalls[i].width;
							m_resolutionOutput[out_iter].height = m_resolutionRenderCalls[i].height;
							m_resolutionOutput[out_iter].calls = (m_resolutionRenderCalls[i].calls > m_resolutionViewportCalls[x].calls) ? m_resolutionRenderCalls[i].calls : m_resolutionViewportCalls[x].calls;
							out_iter++;
							found = true;
							break;
						}
					}
					if (!found && m_resolutionRenderCalls[i].width != 0) {
						m_resolutionOutput[out_iter].width = m_resolutionRenderCalls[i].width;
						m_resolutionOutput[out_iter].height = m_resolutionRenderCalls[i].height;
						m_resolutionOutput[out_iter].calls = m_resolutionRenderCalls[i].calls;
						out_iter++;
					}
					found = false;
					if (out_iter == 8) break;
				}
				if (out_iter < 8) {
					size_t out_iter_s = out_iter;
					for (size_t x = 0; x < 8; x++) {
						for (size_t y = 0; y < out_iter_s; y++) {
							if (m_resolutionViewportCalls[x].width == 0) {
								break;
							}
							if ((m_resolutionViewportCalls[x].width == m_resolutionOutput[y].width) && (m_resolutionViewportCalls[x].height == m_resolutionOutput[y].height)) {
								found = true;
								break;
							}
						}
						if (!found && m_resolutionViewportCalls[x].width != 0) {
							m_resolutionOutput[out_iter].width = m_resolutionViewportCalls[x].width;
							m_resolutionOutput[out_iter].height = m_resolutionViewportCalls[x].height;
							m_resolutionOutput[out_iter].calls = m_resolutionViewportCalls[x].calls;
							out_iter++;
						}
						found = false;
						if (out_iter == 8) break;
					}
				}
				qsort(m_resolutionOutput, 8, sizeof(resolutionCalls), compare);
				if (!m_resolutionOutput[1].width)
					snprintf(Resolutions_c, sizeof(Resolutions_c), "UpdateOneResolutionTitleFullOverlayCustomDrawerText"_tr.c_str(), m_resolutionOutput[0].width, m_resolutionOutput[0].height);
				else snprintf(Resolutions_c, sizeof(Resolutions_c), "UpdateResolutionTitleFullOverlayCustomDrawerText"_tr.c_str(), m_resolutionOutput[0].width, m_resolutionOutput[0].height, m_resolutionOutput[1].width, m_resolutionOutput[1].height);
			}
		}
		else if (!GameRunning && resolutionLookup != 0) {
			resolutionLookup = 0;
		}

		mutexUnlock(&mutex_Misc);

		//Battery Power Flow
		char remainingBatteryLife[8];
		mutexLock(&mutex_BatteryChecker);
		if (batTimeEstimate >= 0) {
			snprintf(remainingBatteryLife, sizeof remainingBatteryLife, "%d:%02d", batTimeEstimate / 60, batTimeEstimate % 60);
		}
		else snprintf(remainingBatteryLife, sizeof remainingBatteryLife, "-:--");
		snprintf(BatteryDraw_c, sizeof BatteryDraw_c, "UpdateBatteryPowerFlowFullOverlayCustomDrawerText"_tr.c_str(), PowerConsumption, remainingBatteryLife);
		mutexUnlock(&mutex_BatteryChecker);
	}

	virtual bool handleInput(uint64_t keysDown, uint64_t keysHeld, const HidTouchState &touchPos, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override {
		if (isKeyComboPressed(keysHeld, keysDown, mappedButtons)) {
			TeslaFPS = 60;
			tsl::goBack();
			return true;
		}
		return false;
	}
};