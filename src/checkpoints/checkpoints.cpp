// Copyright (c) 2018-2019, The Arqma Network
// Copyright (c) 2014-2018, The Monero Project
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

#include "checkpoints.h"

#include "common/dns_utils.h"
#include "string_tools.h"
#include "storages/portable_storage_template_helper.h" // epee json include
#include "serialization/keyvalue_serialization.h"
#include <functional>
#include <vector>

using namespace epee;

#undef MORELO_DEFAULT_LOG_CATEGORY
#define MORELO_DEFAULT_LOG_CATEGORY "checkpoints"

namespace cryptonote
{
  /**
   * @brief struct for loading a checkpoint from json
   */
  struct t_hashline
  {
    uint64_t height; //!< the height of the checkpoint
    std::string hash; //!< the hash for the checkpoint
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(height)
          KV_SERIALIZE(hash)
        END_KV_SERIALIZE_MAP()
  };

  /**
   * @brief struct for loading many checkpoints from json
   */
  struct t_hash_json {
    std::vector<t_hashline> hashlines; //!< the checkpoint lines from the file
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(hashlines)
        END_KV_SERIALIZE_MAP()
  };

  //---------------------------------------------------------------------------
  checkpoints::checkpoints()
  {
  }
  //---------------------------------------------------------------------------
  bool checkpoints::add_checkpoint(uint64_t height, const std::string& hash_str)
  {
    crypto::hash h = crypto::null_hash;
    bool r = epee::string_tools::parse_tpod_from_hex_string(hash_str, h);
    CHECK_AND_ASSERT_MES(r, false, "Failed to parse checkpoint hash string into binary representation!");

    // return false if adding at a height we already have AND the hash is different
    if (m_points.count(height))
    {
      CHECK_AND_ASSERT_MES(h == m_points[height], false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
    }
    m_points[height] = h;
    return true;
  }
  //---------------------------------------------------------------------------
  bool checkpoints::is_in_checkpoint_zone(uint64_t height) const
  {
    return !m_points.empty() && (height <= (--m_points.end())->first);
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h, bool& is_a_checkpoint) const
  {
    auto it = m_points.find(height);
    is_a_checkpoint = it != m_points.end();
    if(!is_a_checkpoint)
      return true;

    if(it->second == h)
    {
      MINFO("CHECKPOINT PASSED FOR HEIGHT " << height << " " << h);
      return true;
    }else
    {
      MWARNING("CHECKPOINT FAILED FOR HEIGHT " << height << ". EXPECTED HASH: " << it->second << ", FETCHED HASH: " << h);
      return false;
    }
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h) const
  {
    bool ignored;
    return check_block(height, h, ignored);
  }
  //---------------------------------------------------------------------------
  //FIXME: is this the desired behavior?
  bool checkpoints::is_alternative_block_allowed(uint64_t blockchain_height, uint64_t block_height) const
  {
    if (0 == block_height)
      return false;

    auto it = m_points.upper_bound(blockchain_height);
    // Is blockchain_height before the first checkpoint?
    if (it == m_points.begin())
      return true;

    --it;
    uint64_t checkpoint_height = it->first;
    return checkpoint_height < block_height;
  }
  //---------------------------------------------------------------------------
  uint64_t checkpoints::get_max_height() const
  {
    if(m_points.empty())
      return 0;
    return m_points.rbegin()->first;
  }
  //---------------------------------------------------------------------------
  const std::map<uint64_t, crypto::hash>& checkpoints::get_points() const
  {
    return m_points;
  }

  bool checkpoints::check_for_conflicts(const checkpoints& other) const
  {
    for (auto& pt : other.get_points())
    {
      if (m_points.count(pt.first))
      {
        CHECK_AND_ASSERT_MES(pt.second == m_points.at(pt.first), false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
      }
    }
    return true;
  }

   bool checkpoints::init_default_checkpoints(network_type nettype)
  {
    if (nettype == TESTNET)
    {

      return true;
    }
    if (nettype == STAGENET)
    {
      return true;
    }

//checkpoints here
    ADD_CHECKPOINT(0, "d12d2eb4b29c62cb9a16be4bb383636e7d6b320967ef74ddd381996148799477");
    ADD_CHECKPOINT(2000, "a41db5720b3967e2b44926629053ad4ece169c83972df2ff80a7e2f31c8897a3");
    ADD_CHECKPOINT(4000, "7b09da7bfa0b79b53f7ca5fc8e5a1b2a7ee3b73b4083efbb5289d8993f929569");
    ADD_CHECKPOINT(6000, "3d7c9e1e1bcde1248618d2f7ed05d90f45032b3c67c27a15968500104c687662");
    ADD_CHECKPOINT(8000, "68927a4cb667a04cf61351fb23131674126b173d75a107863b60beb03561945a");
    ADD_CHECKPOINT(10000, "b50899ce2815a3c61c21333979aa8102d64bf83d564c1f87fb2c259f32563ebd");
    ADD_CHECKPOINT(12000, "b28b5c4eb4f30f5354ca972314457a3f8e7778b724f39a96530ecc3f4bebc895");
    ADD_CHECKPOINT(14000, "fdfc5f4b096fc84bbbf8459aa2577096eb123ba903c3dbc1b3a719d0be3d673a");
    ADD_CHECKPOINT(16000, "4460950caeadf815a1638d106b45aa19e599e4ef09afb0b79ddd32b19053aeda");
    ADD_CHECKPOINT(18000, "a9e32ec1ec81aa214086b3b6e7149423889915e0b8f7410e80604b978b8d2870");
    ADD_CHECKPOINT(20000, "30255ce5cc82bc5172391dff9278f201a13f68f1e69109d2b2688eed36daa44e");
    ADD_CHECKPOINT(22000, "4a0dbbd5b7a97059eedcdcecf5f5050f72d7ad231ea9f7d453c26e08af7eeed4");
    ADD_CHECKPOINT(24000, "a59836232c15e044c7ac1486314f516d981e669851706c3fb49c609bdc0a5ef0");
    ADD_CHECKPOINT(26000, "aa77d96b7efd20fec6b067bdf2459554b6252b4bebf72416d470c7df0d73ff9f");
    ADD_CHECKPOINT(28000, "8017c9374bb6b563b0705c7eced01a7fc961232a8691f1f2f94a9aa70c4d36ce");
    ADD_CHECKPOINT(30000, "bceb662a35f1f8dd8d842b79de93165b642b05c2771ada9fbe480f73694d3080");
    ADD_CHECKPOINT(32000, "45e3a4f3a3934d795aecb10aac7f290dd8f236c1b9e1a0252a270dfbd70ab0a5");
    ADD_CHECKPOINT(34000, "678dda4dad1a905d0bacd636ef7f302c88d763017f79aa533b99994ada5ee199");
    ADD_CHECKPOINT(36000, "5538052409a3d63ea207268c84fb3b8d6ea5d2b5ad4465023c13d5858dc2254f");
    ADD_CHECKPOINT(38000, "863fce29fa5cfb3878f42af917dfdcc510668b0f98b56d433e08e15ffc29acae");
    ADD_CHECKPOINT(40000, "cd1241b22b42a00bdfff5ce9f43b55886b8ae3e234fd731feabb5f02859128c0");
    ADD_CHECKPOINT(42000, "cef853608317da56b541ae845b79364b1a1332366a7f7cccded8b7df18969544");
    ADD_CHECKPOINT(44000, "105f7d0b2f87dd7804c1cba1d99f527cd26427ca1e7a9f338148840826ea4d0b");
    ADD_CHECKPOINT(46000, "050b8c8f031ff9c619760455920444c8ca410b3d39d6aa6297ec095521ab638f");
    ADD_CHECKPOINT(48000, "0f1a03d600005c6c5390f0bb55605fa178c95fded2c3eeb22a5847b6c025698b");
    ADD_CHECKPOINT(50000, "deef740cfe34df6b6802e37c5f962783d5f1567a8b5cd061149728e8f9ab16f2");
    ADD_CHECKPOINT(52000, "c374e1740d42e9bddc059d0015405abbf5a956160f74ed864cdeed6ebfd61cfb");
    ADD_CHECKPOINT(54000, "3b30b08a558f48a12b47fdfcd0f103fb225d166e0ceb3a42abecd7042f520f19");
    ADD_CHECKPOINT(56000, "0db4c3f17ee46c4b7473f4999aaeaaf7db427f6e11a16fe517b324516ae33cdd");
    ADD_CHECKPOINT(58000, "e287da0a403cc2ab7586ab7f47b8bfee6a6f6efc57b25b45bc8e64614c4c6b83");
    ADD_CHECKPOINT(60000, "00332ed95643f342f3aca2deaa88d72a9dd20a4dc0c40ae5971d5a8c172d2adb");
    ADD_CHECKPOINT(62000, "09bba164e23bc3246b590ac9d5c3f1da77d97b9960e22a0775f3ba187cd2d6ed");
    ADD_CHECKPOINT(64000, "cef5bad1ebb1a331af67d5d3ffb82eec19f723f621493dba9f2b67b6235e5071");
    ADD_CHECKPOINT(66000, "125c11a6b14a6c53610bcac203869415702d2af6d4b9d8a7076492d9b9bf2302");
    ADD_CHECKPOINT(68000, "de6c741b77465690c6def34eff08a3e53a43a092f52094a658362648f051aa67");
    ADD_CHECKPOINT(70000, "6e4cdf1403baa51271123d91003dcb9fe1f13e76fe7bcbc2466b28a1dd294485");
    ADD_CHECKPOINT(72000, "71ea1f64c32bfea5ddbe587827f4baaff4428cb74bb24a4d4cafb556f09b0422");
    ADD_CHECKPOINT(74000, "b6b19effe99df81503333b67d2adc14e5b8ede804d3f4e4ffc54275e03c16103");
    ADD_CHECKPOINT(76000, "136e141522fdd707790fb61f5e64c88ea8a87b495cedc5c91bee1baf3fd9c335");
    ADD_CHECKPOINT(78000, "b66cd77e78ea1f0e9100802d35c41843b66352709f5a6e790fb64dfc4ea34805");
    ADD_CHECKPOINT(80000, "fc0894fabeba592e481a81ee7d9d8178943fb38d1ea09e156f3bf9eee9313e6f");
    ADD_CHECKPOINT(82000, "5e8e50ee9c642d77b9594b3fbbb9254f68c4022ac3c75ee5a4f4094101543a84");
    ADD_CHECKPOINT(84000, "9716cdc9fcb323d8e5a016bb0bd62cdd6fdea84b38a2ba636ca9f739122f1f6f");
    ADD_CHECKPOINT(86000, "f3e09ae3a9c90a7edb8ad963644a96f80b38e4c656f30edc13bdcbb493b204d8");
    ADD_CHECKPOINT(88000, "cce48663fb3627abcb261a28adcd802ab628fd4f4445d69cf82f581598ec5bc3");
    ADD_CHECKPOINT(90000, "188018368d93ba683c50646d30bea06dd047a0df52f05a9c09bb4915ef2432b0");
    ADD_CHECKPOINT(92000, "94206d317ce85adccceec5c581e34bd91804d046c2afcacf41b85928b7d9154a");
    ADD_CHECKPOINT(94000, "1b771ad498ca278c0418dde7bb7101cef749cda470812cbdbe759c4b9fb3a064");
    ADD_CHECKPOINT(96000, "a0554af75c76f0dffe5ff927c7beefaafa3359da238d4bf610e0af771ac02a6d");
    ADD_CHECKPOINT(98000, "fff0f61a474183908226d1bd1dee73d0673337a3746e7e156b8a300606498875");
    ADD_CHECKPOINT(100000, "eaf4a24951ce31ebfb58caf03a3b2868adeeb0b47b9b7a843bc7b4c7c7dee23b");
    ADD_CHECKPOINT(102000, "4a561d7741b7d891b811d4680643a6624af8e131540afadcde5b9d45f123951e");
    ADD_CHECKPOINT(104000, "5e8984b7ba0c3201a233babaf6431ecbffa7696f8353d1e67b391ee79a1169e5");
    ADD_CHECKPOINT(106000, "4c1215450a5f0d5061548cfff365662ddf6ece3e43da79e4e5a4edc1e9f9ca6c");
    ADD_CHECKPOINT(108000, "4b1cd5674ec5fb4dd30e2d4ce159df9406a87fa308fa6431b40265207cd430eb");
    ADD_CHECKPOINT(110000, "d98fac4ceecb675191a3d5c7d372d9040d75b77a7e1650ffb06edc9b633d8acb");
    ADD_CHECKPOINT(112000, "93b0d19a80c0a9eb0716dc20e294429b1e8ea4699aa72716dece7a47a695df4c");
    ADD_CHECKPOINT(114000, "7e044c375363bd1547cd03c51164161463513f612551511f1f8d2656948a67c1");
    ADD_CHECKPOINT(116000, "c57b699aaee353d7402bcc532a11587e4e25b8bb98670663265d957cd5166476");
    ADD_CHECKPOINT(118000, "64649b6ae7dc5088151f90379fdbf187ed8f2f696b58aad09fd295de4434ea67");
    ADD_CHECKPOINT(120000, "3b8189e629372081d3f1c4433b0f5dce37d9515423349f5278108125c1d3a097");
    ADD_CHECKPOINT(122000, "9f93a75e63849a371fd7916a8ad6fd85be119015504c99222c07dcd5400bef28");
    ADD_CHECKPOINT(124000, "5d6463405c9abb21cc0a66c82f3aea6256c06293e0bda3e737f4ead1b496cb8c");
    ADD_CHECKPOINT(126000, "2c2a3fe7122e63ca0f246f28f4f5b84d01cb6c69c0b4753e75e620917d92779c");
    ADD_CHECKPOINT(128000, "a95ce93cc2cdadc89206f9b3110d0b6e0fa325218384a666e115fd94f3c86ace");
    ADD_CHECKPOINT(130000, "ccbcae4b030798185e510590740c959bbcf61db49a2cec58b01b3cf985896f41");
    ADD_CHECKPOINT(132000, "4369056261fc68c5563cd7e12b52a39370fa03442c6105c26459f1a6172be66f");
    ADD_CHECKPOINT(134000, "e0c94d36b7e817a0ff6041e70723c7d78e39889fb3027132d29cd2a5ddf24274");
    ADD_CHECKPOINT(136000, "eb5b0bfc6225156935325be8fce10d9f1299d134bb70b84b0fc0c84daea11e75");
    ADD_CHECKPOINT(138000, "5f3483d87ee2d834278ac1fc4ca95e3a0bef379a13dfa2e427c32efc4c9f107a");
    ADD_CHECKPOINT(140000, "e9e7a30ec068069b046a2b12aa5af0b131a0e6e54efba51ff71edfcf535a6917");
    ADD_CHECKPOINT(142000, "e03b458fc14ee44960548efd2b6ed470d63eaebdba4f9b38209723f98ec87192");
    ADD_CHECKPOINT(144000, "f6a1c9d8deb21985c1242b6274089626f518352a71bafc53abcd1aa2abb66277");
    ADD_CHECKPOINT(146000, "ab489617a2b1e2fb19df94c3c5f305a5412b072ae9a56b03693837ca13bcf102");
    ADD_CHECKPOINT(148000, "1787a5c7090b39d10ea3441733534f63e69f7b67512497e6aa4886a9c69d694d");
    ADD_CHECKPOINT(150000, "e004c5ffc50b2ae7f62063c12c03f5f6f3c49528d3b64425b7e47464513b2458");
    ADD_CHECKPOINT(152000, "c756e5ed8a24b1d78f0169a22d46197dc0cc4088a09f6ed82f4e97208696b6e3");
    ADD_CHECKPOINT(154000, "40dda1259eacbcff68d19bd9cbb9d1724b9f914255827c59a12217666b02a16d");
    ADD_CHECKPOINT(156000, "9c720da08677d63efe0554f78d43294610e2a02dcead0167d4a336ede844f3e7");

    return true;
  }

  bool checkpoints::load_checkpoints_from_json(const std::string &json_hashfile_fullpath)
  {
    boost::system::error_code errcode;
    if (! (boost::filesystem::exists(json_hashfile_fullpath, errcode)))
    {
      LOG_PRINT_L1("Blockchain checkpoints file not found");
      return true;
    }

    LOG_PRINT_L1("Adding checkpoints from blockchain hashfile");

    uint64_t prev_max_height = get_max_height();
    LOG_PRINT_L1("Hard-coded max checkpoint height is " << prev_max_height);
    t_hash_json hashes;
    if (!epee::serialization::load_t_from_json_file(hashes, json_hashfile_fullpath))
    {
      MERROR("Error loading checkpoints from " << json_hashfile_fullpath);
      return false;
    }
    for (std::vector<t_hashline>::const_iterator it = hashes.hashlines.begin(); it != hashes.hashlines.end(); )
    {
      uint64_t height;
      height = it->height;
      if (height <= prev_max_height) {
	LOG_PRINT_L1("ignoring checkpoint height " << height);
      } else {
	std::string blockhash = it->hash;
	LOG_PRINT_L1("Adding checkpoint height " << height << ", hash=" << blockhash);
	ADD_CHECKPOINT(height, blockhash);
      }
      ++it;
    }

    return true;
  }

  bool checkpoints::load_checkpoints_from_dns(network_type nettype)
  {
    std::vector<std::string> records;

    // All four ArQ-Net domains have DNSSEC on and valid
    static const std::vector<std::string> dns_urls = {

	};

    static const std::vector<std::string> testnet_dns_urls = {
    };

    static const std::vector<std::string> stagenet_dns_urls = {
    };

    if (!tools::dns_utils::load_txt_records_from_dns(records, nettype == TESTNET ? testnet_dns_urls : nettype == STAGENET ? stagenet_dns_urls : dns_urls))
      return true; // why true ?

    for (const auto& record : records)
    {
      auto pos = record.find(":");
      if (pos != std::string::npos)
      {
        uint64_t height;
        crypto::hash hash;

        // parse the first part as uint64_t,
        // if this fails move on to the next record
        std::stringstream ss(record.substr(0, pos));
        if (!(ss >> height))
        {
    continue;
        }

        // parse the second part as crypto::hash,
        // if this fails move on to the next record
        std::string hashStr = record.substr(pos + 1);
        if (!epee::string_tools::parse_tpod_from_hex_string(hashStr, hash))
        {
    continue;
        }

        ADD_CHECKPOINT(height, hashStr);
      }
    }
    return false;
  }

  bool checkpoints::load_new_checkpoints(const std::string &json_hashfile_fullpath, network_type nettype, bool dns)
  {
    bool result;

    result = load_checkpoints_from_json(json_hashfile_fullpath);
    if (dns)
    {
      result &= load_checkpoints_from_dns(nettype);
    }

    return result;
  }
}
