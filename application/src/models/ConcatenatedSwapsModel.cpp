#include "ConcatenatedSwapsModel.h"

namespace XWallet::Models {

ConcatenatedSwapsModel::ConcatenatedSwapsModel(QObject* parent)
    : QConcatenateTablesProxyModel(parent)
{
    connect(this, &ConcatenatedSwapsModel::modelReset, this, &ConcatenatedSwapsModel::lengthChanged);
    connect(this, &ConcatenatedSwapsModel::rowsInserted, this, &ConcatenatedSwapsModel::lengthChanged);
    connect(this, &ConcatenatedSwapsModel::rowsRemoved, this, &ConcatenatedSwapsModel::lengthChanged);
}

QHash<int, QByteArray> ConcatenatedSwapsModel::roleNames() const
{
    return {
        { SwapRole,          "swap"          },
        { BidAssetRole,      "bidAsset"      },
        { AskAssetRole,      "askAsset"      },
        { BidCoinAmountRole, "bidCoinAmount" },
        { BidFiatAmountRole, "bidFiatAmount" },
        { AskCoinAmountRole, "askCoinAmount" },
        { AskFiatAmountRole, "askFiatAmount" },
        { ExpandedRole,      "expanded"      },
        { ShowLogsRole,      "showLogs"      },
    };
}

} // namespace XWallet::Models
