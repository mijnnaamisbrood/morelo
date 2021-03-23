// Copyright (c) 2021-2021, WallStreetBets Coin - WSBC
// Copyright (c) 2019-2020, The Wallstreetbets Network
// Copyright (c) 2018-2020, The Arqma Network
// Copyright (c) 2014-2020, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <stdexcept>
#include <string>
#include <boost/uuid/uuid.hpp>

#define CRYPTONOTE_DNS_TIMEOUT_MS                       20000

#define CRYPTONOTE_MAX_BLOCK_NUMBER                     500000000
#define CRYPTONOTE_PUBLIC_ADDRESS_TEXTBLOB_VER          0
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V2           600
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V3           300
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V4           CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V3
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT              240

// MONEY_SUPPLY - total number coins to be generated
#define MONEY_SUPPLY                                    ((uint64_t)100000000000000000)
#define EMISSION_SPEED_FACTOR_PER_MINUTE                (20)
#define EMISSION_SPEED_FACTOR_V16                       (20)
#define FINAL_SUBSIDY_PER_MINUTE                        ((uint64_t)300000000)


#define CRYPTONOTE_REWARD_BLOCKS_WINDOW                 100
#define CRYPTONOTE_LONG_TERM_BLOCK_WEIGHT_WINDOW_SIZE   100000 // size in blocks of the long term block weight median window
#define CRYPTONOTE_SHORT_TERM_BLOCK_WEIGHT_SURGE_FACTOR 50
#define CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE          600
#define CRYPTONOTE_DISPLAY_DECIMAL_POINT                9
// COIN - number of smallest units in one coin
#define COIN                                            ((uint64_t)1000000000)

#define DYNAMIC_FEE_PER_KB_BASE_BLOCK_REWARD            ((uint64_t)10000000000)
#define DYNAMIC_FEE_PER_BYTE                            ((uint64_t)200)
#define DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT        ((uint64_t)750)

#define ORPHANED_BLOCKS_MAX_COUNT                       100


#define DIFFICULTY_TARGET_V3                            DIFFICULTY_TARGET_V2
#define DIFFICULTY_TARGET_V2                            240  // seconds
#define DIFFICULTY_TARGET_V1                            120  // seconds - before first fork
#define DIFFICULTY_WINDOW                               720 // blocks
#define DIFFICULTY_WINDOW_V2                            30
#define DIFFICULTY_WINDOW_V3                            17
#define DIFFICULTY_LAG                                  15  // !!!
#define DIFFICULTY_CUT                                  60  // timestamps to cut after sorting

#define DIFFICULTY_TARGET_V10                           120
#define DIFFICULTY_BLOCKS_COUNT_V10                     (DIFFICULTY_WINDOW_V10 + 1)
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V10          360
#define DIFFICULTY_WINDOW_V10                           90

#define DIFFICULTY_TARGET_V11                           120
#define DIFFICULTY_BLOCKS_COUNT_V11                     (DIFFICULTY_WINDOW_V11 + 1)
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V11          360
#define DIFFICULTY_WINDOW_V11                           90

#define DIFFICULTY_TARGET_V16                           60
#define DIFFICULTY_WINDOW_V16                           45
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V16          180
#define DIFFICULTY_BLOCKS_COUNT_V16                     (DIFFICULTY_WINDOW_V16 + 1)

#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW               11

#define DIFFICULTY_BLOCKS_COUNT_V3                      (DIFFICULTY_WINDOW_V3 + 1)
#define DIFFICULTY_BLOCKS_COUNT_V2                      (DIFFICULTY_WINDOW_V2 + 1) // added to make N=N
#define DIFFICULTY_BLOCKS_COUNT                         DIFFICULTY_WINDOW + DIFFICULTY_LAG

#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V1   DIFFICULTY_TARGET_V11 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V2   DIFFICULTY_TARGET_V16 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS       1

#define DIFFICULTY_BLOCKS_ESTIMATE_TIMESPAN             DIFFICULTY_TARGET_V2 //just alias; used by tests

#define BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT          10000  //by default, blocks ids count in synchronizing

#define BLOCKS_SYNCHRONIZING_MAX_COUNT                  2048 //must be a power of 2, greater than 128, equal to SEEDHASH_EPOCH_BLOCKS

#define CRYPTONOTE_MEMPOOL_TX_LIVETIME                  (86400*3) //seconds, three days
#define CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME   604800 //seconds, one week

#define COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT           1000

#define P2P_LOCAL_WHITE_PEERLIST_LIMIT                  1000
#define P2P_LOCAL_GRAY_PEERLIST_LIMIT                   5000

#define P2P_DEFAULT_CONNECTIONS_COUNT                   16
#define P2P_DEFAULT_CONNECTIONS_COUNT_STAGENET          4
#define P2P_DEFAULT_HANDSHAKE_INTERVAL                  60         // secondes
#define P2P_DEFAULT_PACKET_MAX_SIZE                     50000000   // 50000000 bytes maximum packet size
#define P2P_DEFAULT_PEERS_IN_HANDSHAKE                  250
#define P2P_DEFAULT_CONNECTION_TIMEOUT                  5000       // 5 seconds
#define P2P_DEFAULT_SOCKS_CONNECT_TIMEOUT               45         // seconds
#define P2P_DEFAULT_PING_CONNECTION_TIMEOUT             5000       // 5 seconds
#define P2P_DEFAULT_INVOKE_TIMEOUT                      60*2*1000  // 2 minutes
#define P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT            5000       // 5 seconds
#define P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT       70
#define P2P_DEFAULT_ANCHOR_CONNECTIONS_COUNT            2
#define P2P_DEFAULT_SYNC_SEARCH_CONNECTIONS_COUNT       2

#define P2P_DEFAULT_LIMIT_RATE_UP                       4096       // Kbps
#define P2P_DEFAULT_LIMIT_RATE_DOWN                     16384      // Kbps

#define P2P_FAILED_ADDR_FORGET_SECONDS                  86400      // 1 day
#define P2P_IP_BLOCKTIME                                172800     // 48 hours
#define P2P_IP_FAILS_BEFORE_BLOCK                       10
#define P2P_IDLE_CONNECTION_KILL_INTERVAL               60         // 60 seconds

#define P2P_SUPPORT_FLAG_FLUFFY_BLOCKS                  0x01
#define P2P_SUPPORT_FLAGS                               P2P_SUPPORT_FLAG_FLUFFY_BLOCKS

#define ALLOW_DEBUG_COMMANDS

#define CRYPTONOTE_NAME                                 "wallstreetbets"
#define CRYPTONOTE_POOLDATA_FILENAME                    "poolstate.bin"
#define CRYPTONOTE_BLOCKCHAINDATA_FILENAME              "data.mdb"
#define CRYPTONOTE_BLOCKCHAINDATA_LOCK_FILENAME         "lock.mdb"
#define P2P_NET_DATA_FILENAME                           "p2pstate.bin"
#define RPC_PAYMENTS_DATA_FILENAME                      "rpcpayments.bin"
#define MINER_CONFIG_FILE_NAME                          "miner_conf.json"

#define HF_VERSION_LONG_TERM_BLOCK_WEIGHT               15

#define THREAD_STACK_SIZE                               10 * 1024 * 1024

#define PER_KB_FEE_QUANTIZATION_DECIMALS                8

#define HASH_OF_HASHES_STEP                             256

#define DEFAULT_TXPOOL_MAX_WEIGHT                       648000000ull // 3 days at 300000, in bytes

#define BULLETPROOF_MAX_OUTPUTS                         16

#define CRYPTONOTE_PRUNING_STRIPE_SIZE                  4096         // the smaller, the smoother the increase
#define CRYPTONOTE_PRUNING_LOG_STRIPES                  3            // the higher, the more space saved
#define CRYPTONOTE_PRUNING_TIP_BLOCKS                   5500         // the smaller, the more space saved
//#define CRYPTONOTE_PRUNING_DEBUG_SPOOF_SEED

#define RPC_CREDITS_PER_HASH_SCALE                      ((float)(1<<24))

static constexpr uint64_t POISSON_CHECK_TRIGGER = 5;  // Reorg size that triggers poisson timestamp check
static constexpr uint64_t POISSON_CHECK_DEPTH = 128;  // Main-chain depth of the poisson check. The attacker will have to tamper 50% of those blocks
static constexpr double POISSON_LOG_P_REJECT = -75.0; // Reject reorg if the probablity that the timestamps are genuine is below e^x, -75 = 10^-33

// New constants are intended to go here
namespace config
{
   uint64_t const DEFAULT_DUST_THRESHOLD = 0; // Deprecated
   std::string const P2P_REMOTE_DEBUG_TRUSTED_PUB_KEY = "0000000000000000000000000000000000000000000000000000000000000000";

   uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x233caf; // Wallet prefix: WSBC MainNet
   uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0xda2bcaf; // Wallet prefix: WSBC2 MainNet
   uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0xec2bcaf; // Wallet prefix: WSBC3 MainNet
   uint16_t const P2P_DEFAULT_PORT = 17761;
   uint16_t const RPC_DEFAULT_PORT = 17762;
   uint16_t const ZMQ_DEFAULT_PORT = 18891;
   boost::uuids::uuid const NETWORK_ID = { {
       0x21, 0x18, 0x66, 0x85, 0xa3, 0xF5, 0x11, 0x78, 0x82, 0x83, 0xFF, 0xFF, 0xFF, 0x33, 0x73, 0x2B
     } }; //
   std::string const GENESIS_TX = "011201ff00011e026bc5c7db8a664f652d78adb587ac4d759c6757258b64ef9cba3c0354e64fb2e42101abca6a39c561d0897be183eb0143990eba201aa7d2c652ab0555d28bb4b70728";
   uint32_t const GENESIS_NONCE = 17761;

   namespace testnet
   {
     uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0xfe2bcaf; // Wallet prefix: WSBC4... // decimal prefix: 21450
     uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0x1202bcaf; // Wallet prefix: WSBC5... // decimal prefix: 20554
     uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0x1322bcaf; // Wallet prefix: WSBC6... // decimal prefix: 21066
     uint16_t const P2P_DEFAULT_PORT = 16671;
     uint16_t const RPC_DEFAULT_PORT = 16672;
     uint16_t const ZMQ_DEFAULT_PORT = 19981;
     boost::uuids::uuid const NETWORK_ID = { {
         0x25, 0x83, 0x15, 0x85, 0xFF, 0xFF, 0xFF, 0x73, 0x35, 0x85, 0xFF, 0xFF, 0xFF, 0x71, 0x02, 0x2B
       } }; // TEST
   }

   namespace stagenet
   {
     uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x1422bcaf; // Wallet prefix: WSBC7... // decimal prefix: 14794
     uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0x1542bcaf; // Wallet prefix: WSBC8... // decimal prefix: 1524426
     uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0x1662bcaf; // Wallet prefix: WSBC9... // decimal prefix: 1934538
     uint16_t const P2P_DEFAULT_PORT = 15561;
     uint16_t const RPC_DEFAULT_PORT = 15562;
     uint16_t const ZMQ_DEFAULT_PORT = 13341;
     boost::uuids::uuid const NETWORK_ID = { {
         0x78, 0x85, 0x78, 0x25, 0xFF, 0xFF, 0xFF, 0x33, 0x38, 0x53, 0xFF, 0xFF, 0xFF, 0x58, 0x82, 0x4C
       } }; // STAGE
   }

   namespace blockchain_settings
   {
     const uint64_t WALLSTREETBETS_PREMINE = 14000000000000000;
     const uint64_t PREMINE_BURN = 0; // Will need to be set after knowing exact amount.
     const uint64_t old_block_min_size = 1048576;
     const uint64_t new_block_min_size = 262144;
     const uint8_t WALLSTREETBETS_GENESIS_BLOCK_MAJOR_VERSION = 1;
     const uint8_t WALLSTREETBETS_GENESIS_BLOCK_MINOR_VERSION = 1;
     const uint8_t WALLSTREETBETS_BLOCK_UNLOCK_CONFIRMATIONS = 18; // How many blocks mined are needed to unlock block_reward.
   }

   namespace tx_settings
   {
     const uint8_t WALLSTREETBETS_TX_CONFIRMATIONS_REQUIRED = 4; // How many blocks are needed to confirm transaction sent.
     const uint8_t CURRENT_TX_VERSION = 2; // Current Transaction Version Valid on Arq-Net
     const uint64_t old_tx_size_limit = 49152;
     const uint64_t new_tx_size_limit = 102400;
     const uint64_t max_tx_in_block = 1024;
     const uint64_t default_mixin = 10;
     const uint64_t default_ringsize = default_mixin + 1;
   }

   namespace sync
   {
     const uint64_t HIGHEST_CHECKPOINT = 17920; // Blockchain height up to Rapid_Sync is ON.
     const size_t NORMAL_SYNC = 20; // Amount of Blocks to download and Validate at ones while Synchronizung with Wallstreetbets Network.
     const size_t RAPID_SYNC = 100; // Amount of Blocks to download at ones from already known by Wallstreetbets Daemon and Checkpoint are hardcoded into codebase
   }

   namespace governance
   {
     static constexpr const char* MAINNET_WALLET_ADDRESS = "WSBCodZd65aic5fVDh3M84VLEpb3vRtn3FKx2nbLcsJW1oHWNy39JtJG6M96z62zB7PkKcAdAAfVvfQM3Dr4PFwAftDYY5bWcwQ";
     static constexpr const char* TESTNET_WALLET_ADDRESS = "";
     static constexpr const char* STAGENET_WALLET_ADDRESS = "WSBC7uHpisnXMoKqQA7BzV7CqPAh9a4NFP68x7XNxbFB15YfAa3TyAq4cKbUTKTPdngdoSyEnTdzoCg8evVP6vnngKCLXi1qGk44g";
   }
}

namespace wsbc
{
  const char *const mainnet_core_nodes[] =
  {
    "209.126.8.14"/* Node_1 */,
    "209.126.8.16"/* Node_2 */,
    "209.126.8.219"/* Node_3 */,
    "209.126.8.13"/* Node_4 */,
    "62.171.171.201"/* Node_5 */,
    "62.171.171.195"/* Node_6 */,
    "144.91.113.223"/* Node_7 */,
    "167.86.113.14"/* Node_8 */,
    "82.44.107.19",
    "161.97.102.172"
  };

  const char *const testnet_core_nodes[] =
  {
  };

  const char *const stagenet_core_nodes[] =
  {
    "82.44.107.19",
    "161.97.102.172",
    "37.247.209.235"
  };
}

namespace cryptonote
{
  enum network_type : uint8_t
    {
      MAINNET = 0,
      TESTNET,
      STAGENET,
      FAKECHAIN,
      UNDEFINED = 255
    };
    struct config_t
    {
      uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX;
      uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX;
      uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX;
      uint16_t const P2P_DEFAULT_PORT;
      uint16_t const RPC_DEFAULT_PORT;
      uint16_t const ZMQ_DEFAULT_PORT;
      boost::uuids::uuid const NETWORK_ID;
      std::string const GENESIS_TX;
      uint32_t const GENESIS_NONCE;
    };
    inline const config_t& get_config(network_type nettype)
    {
      static const config_t mainnet = {
        ::config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
        ::config::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
        ::config::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
        ::config::P2P_DEFAULT_PORT,
        ::config::RPC_DEFAULT_PORT,
        ::config::ZMQ_DEFAULT_PORT,
        ::config::NETWORK_ID,
        ::config::GENESIS_TX,
        ::config::GENESIS_NONCE
      };
      static const config_t testnet = {
        ::config::testnet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
        ::config::testnet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
        ::config::testnet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
        ::config::testnet::P2P_DEFAULT_PORT,
        ::config::testnet::RPC_DEFAULT_PORT,
        ::config::testnet::ZMQ_DEFAULT_PORT,
        ::config::testnet::NETWORK_ID,
        ::config::GENESIS_TX,
        ::config::GENESIS_NONCE
      };
      static const config_t stagenet = {
        ::config::stagenet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
        ::config::stagenet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
        ::config::stagenet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
        ::config::stagenet::P2P_DEFAULT_PORT,
        ::config::stagenet::RPC_DEFAULT_PORT,
        ::config::stagenet::ZMQ_DEFAULT_PORT,
        ::config::stagenet::NETWORK_ID,
        ::config::GENESIS_TX,
        ::config::GENESIS_NONCE
      };
      switch (nettype)
      {
        case MAINNET: return mainnet;
        case TESTNET: return testnet;
        case STAGENET: return stagenet;
        case FAKECHAIN: return mainnet;
        default: throw std::runtime_error("Invalid network type");
      }
   };
}
