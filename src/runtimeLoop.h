#pragma once

using RuntimeCallback = void (*)();

struct RuntimeCallbacks {
    RuntimeCallback wifi;
    RuntimeCallback configChange;
    RuntimeCallback sensor;
    RuntimeCallback temperature;
    RuntimeCallback modbus;
    RuntimeCallback victron;
    RuntimeCallback display;
};

inline void runRuntimeLoop(const RuntimeCallbacks& callbacks, bool& configChanged) {
    if (callbacks.wifi != nullptr) {
        callbacks.wifi();
    }

    if (configChanged) {
        if (callbacks.configChange != nullptr) {
            callbacks.configChange();
        }
        configChanged = false;
    }

    if (callbacks.sensor != nullptr) {
        callbacks.sensor();
    }
    if (callbacks.temperature != nullptr) {
        callbacks.temperature();
    }
    if (callbacks.modbus != nullptr) {
        callbacks.modbus();
    }
    if (callbacks.victron != nullptr) {
        callbacks.victron();
    }
    if (callbacks.display != nullptr) {
        callbacks.display();
    }
}