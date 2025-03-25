#include "risk_manager.h"

namespace Trading {
    RiskManager::RiskManager(Common::Logger *logger, const Trading::PositionKeeper *position_keeper,
                             const Common::TradeEngineCfgHashMap &ticker_cfg)
        : logger_(logger) {
        for (TickerId i = 0; i < ME_MAX_TICKERS; ++i)  {
            ticker_risk_.at(i).position_info_ = position_keeper->getPositionInfo(i);
            ticker_risk_.at(i).risk_cfg_ = ticker_cfg[i].risk_cfg_;
        }
    }
}