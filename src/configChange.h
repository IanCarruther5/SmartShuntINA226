#pragma once

inline bool shouldApplyConfigChange(bool paramsChanged) {
    return paramsChanged;
}

inline bool handleConfigChange(bool& paramsChanged) {
    if (!shouldApplyConfigChange(paramsChanged)) {
        return false;
    }

    paramsChanged = false;
    return true;
}