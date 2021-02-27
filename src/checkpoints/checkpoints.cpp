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

#undef WALLSTREETBETS_DEFAULT_LOG_CATEGORY
#define WALLSTREETBETS_DEFAULT_LOG_CATEGORY "checkpoints"

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
  ADD_CHECKPOINT(0, "3260f61cf521e962d0f5728b61e4a9fa327df8e951a9fa5f7d824e323b17a8ae");
  ADD_CHECKPOINT(100, "f055899a6d9b85bfd4172dc7c119df98a99989c6d12ade3436bae5e1148193d4");
  ADD_CHECKPOINT(200, "eed9358d5842add59a0d992833b7b97ef4f34a8a28e92c61e910ad7335af6190");
  ADD_CHECKPOINT(300, "7d2c7884b88caa357f98466c555a0d3872f05c99f648b2b7eb54d2c92a57f7a1");
  ADD_CHECKPOINT(400, "48237eea5065f8f2c12457436d136427d7d88ec7c036d7a3bd4ccbac07348818");
  ADD_CHECKPOINT(500, "00ba8207326830446ebf3afc7f3557f49ac886bd7f55c89409e70c6a9fa978ee");
  ADD_CHECKPOINT(600, "bdcf8e3f44f1f12f46a08330fe6e0e85e8dd0e3021e402c5ccc5132c746eaebe");
  ADD_CHECKPOINT(700, "585cbb628566a51005b4c101fb147447b7959395928fdb59d1ceb9f8b8dcdb9c");
  ADD_CHECKPOINT(800, "000fadad1ef264370543ae0cf9e5d920ce9d42e935d11aea69c5444417d8795f");
  ADD_CHECKPOINT(900, "fb830cdc0962f2134113ff11056f09176b79e8ec02723302b78f8c8cd65165ed");
  ADD_CHECKPOINT(1000, "a1a1a3c2c26dccd931354002ea9db642b954119723c66c95ae793f45099ebd2f");
  ADD_CHECKPOINT(1100, "5a62537a9a04ad365e7a1c728e2c53177a362199b07b3f6eebecf59391db9b1e");
  ADD_CHECKPOINT(1200, "b2fb3b2660117b6486a26ecbe0a6f34aa5d2070651a38c042e4e2d6164d68bb2");
  ADD_CHECKPOINT(1300, "6a31d6d017fd365105f54ec5d6084f7cd0bf8e1dd2b0097a19a7ce29caaabbe5");
  ADD_CHECKPOINT(1400, "f7d51c9ec01002add954d3693cc5ee64319dbe007dc6c863c92dab130e4fc6e3");
  ADD_CHECKPOINT(1500, "d2865345b59952f90981c57b6b886f22703904a5687c59322b07e52e7b722152");
  ADD_CHECKPOINT(1600, "e24399e4b2f5ad34a43720be453b223cf910548ac37d08cb27d0b98600d3ab22");
  ADD_CHECKPOINT(1700, "ea0d02c830348d11d4dd6af84a405a27e89bd370ed37297247837067c61eb407");
  ADD_CHECKPOINT(1800, "11df1e3c4d792caaf4c2e09004efa1502b1f3e2ec777493aa7a45b618326c59c");
  ADD_CHECKPOINT(1900, "ba8247f8283108ec1d8a3405242609a949706a3fea7e38be9da46e0781b7b00a");
  ADD_CHECKPOINT(2000, "3253902ac664656a7d7413d552f3a49b2a00730882fa900d378f6336711f26fc");
  ADD_CHECKPOINT(2100, "3ab67905da57a7e034107e53be5ae8cf0b46a90419a4e4d0bf07d5913eb72a72");
  ADD_CHECKPOINT(2200, "b8eff6f19eaf5c1be6ed7c585636d560cc2899755d6604219d431319a523e929");
  ADD_CHECKPOINT(2300, "42e1b231ad3577af3f5a8b6b9460b93c828282ed92618cbaa4944301b2e5d455");
  ADD_CHECKPOINT(2400, "a6820aae7ed7a020ebb7af595b453d2dbeffd8af727c3805d3ea9d8e14c77902");
  ADD_CHECKPOINT(2500, "ebd4eb8d61eb98b16a66382f2e767412f6320023306276057f4b54aad758b6f3");
  ADD_CHECKPOINT(2600, "e0b21166c3adbd45d443aa393c4863e0dddf04986dcf55f4ae00fdbb18c93e32");
  ADD_CHECKPOINT(2700, "972f7ee00f2a8a66b0019dfcb5dc5dc2cada1a4b709127ca0386d9ca7791bb12");
  ADD_CHECKPOINT(2800, "6d9c8a915e09c9408ff19a92bc3bbe85df2ea7085e27b941140057e23657ec47");
  ADD_CHECKPOINT(2900, "1b29d3f3ad061d501be886785dccf49cbc6416a7afb04ef8881cc0915ad8f9fe");
  ADD_CHECKPOINT(3000, "c7e2c3172dc99c5ee35c05887ea47ee231147f94b474d5b2d216648ecdf79312");
  ADD_CHECKPOINT(3100, "ca3eb5ba53c352b1c861b8ea91c31ffedfd81d534e871b769a0d3a2f4a86f864");
  ADD_CHECKPOINT(3200, "0ad306f997bb5b135c400d8bddad3ddaf6e3667ab8a650b56533d5dc48eb2b69");
  ADD_CHECKPOINT(3300, "fd7bec61d77790cc554c123672c07441a1a7c52e822dfb0239997f4f1f8958c3");
  ADD_CHECKPOINT(3400, "d67fb3c075d8d8392375a1652cfefdfd347e540d869f7e2dc2114df7c1e87939");
  ADD_CHECKPOINT(3500, "2649b87b37eb4b7d2bf78844eeaa773d645ea1625f9f5e96898b675ec9adc20e");
  ADD_CHECKPOINT(3600, "21f5f8383d414c86ae19fcc7f6b1d17905e0723feb6740c9663ad8c0663c8bad");
  ADD_CHECKPOINT(3700, "23cb5c4da02789d55c22efa5c18e30e60f456a521ba9b6c32dafd1273eae803c");
  ADD_CHECKPOINT(3800, "c3f48a96421238440d030c5c9cc335799cdb1b0b6a50ff87800c7adcb1f8c111");
  ADD_CHECKPOINT(3900, "aa97d4e1ca02c166cf34ad465df27befd5f68cb61ad5e231b7909f4551785c98");
  ADD_CHECKPOINT(4000, "7fe72ac78acbfbbfe7a2a23299b6bd7db67b1cc11832c47e62b0534ce3af81d9");
  ADD_CHECKPOINT(4100, "b384849071dc07aa9f087494da13be0eda287c15149673787f741ef667897a0a");
  ADD_CHECKPOINT(4200, "53aea1a8db5c559dfb5ae0364b909b8c163fbed471547ec7673ff9d7db48103e");
  ADD_CHECKPOINT(4300, "eda6d1b42c4e1bcf638e0c76604715a650c39ef4f8b14d05ccfa142fb2c9e3f3");
  ADD_CHECKPOINT(4400, "4b4370917e1c29ad242a31c956b23653cc00a3039bb3047cef5e93f119f29c11");
  ADD_CHECKPOINT(4500, "6fef0e0e7d4054286b7a42d315b73b84cdcc91613f91f15e880e7a8ca7683599");
  ADD_CHECKPOINT(4600, "ed991f1f5105824b4bca7c18a3328230feec5e04374b712d044a1536f68c00b2");
  ADD_CHECKPOINT(7000, "85ffcbd841b6b243ecc8a84e632d9164704d3ef7305ecff6fd4a7a9a812200e7");

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
