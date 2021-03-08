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
    bool r = epee::string_tools::hex_to_pod(hash_str, h);
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
      ADD_CHECKPOINT(0, "3260f61cf521e962d0f5728b61e4a9fa327df8e951a9fa5f7d824e323b17a8ae");

      return true;
    }
    if (nettype == STAGENET)
    {
      ADD_CHECKPOINT(0, "3260f61cf521e962d0f5728b61e4a9fa327df8e951a9fa5f7d824e323b17a8ae");

      return true;
    }

//checkpoints here
    ADD_CHECKPOINT(0, "3260f61cf521e962d0f5728b61e4a9fa327df8e951a9fa5f7d824e323b17a8ae");
    ADD_CHECKPOINT(1, "e30f8be3f0f59cf88cd602914071ee7f79f7353ab017d89b40a6d666bf769655");
    ADD_CHECKPOINT(500, "00ba8207326830446ebf3afc7f3557f49ac886bd7f55c89409e70c6a9fa978ee");
    ADD_CHECKPOINT(1000, "a1a1a3c2c26dccd931354002ea9db642b954119723c66c95ae793f45099ebd2f");
    ADD_CHECKPOINT(1500, "d2865345b59952f90981c57b6b886f22703904a5687c59322b07e52e7b722152");
    ADD_CHECKPOINT(2000, "3253902ac664656a7d7413d552f3a49b2a00730882fa900d378f6336711f26fc");
    ADD_CHECKPOINT(2500, "ebd4eb8d61eb98b16a66382f2e767412f6320023306276057f4b54aad758b6f3");
    ADD_CHECKPOINT(3000, "c7e2c3172dc99c5ee35c05887ea47ee231147f94b474d5b2d216648ecdf79312");
    ADD_CHECKPOINT(3500, "2649b87b37eb4b7d2bf78844eeaa773d645ea1625f9f5e96898b675ec9adc20e");
    ADD_CHECKPOINT(4000, "7fe72ac78acbfbbfe7a2a23299b6bd7db67b1cc11832c47e62b0534ce3af81d9");
    ADD_CHECKPOINT(4500, "6fef0e0e7d4054286b7a42d315b73b84cdcc91613f91f15e880e7a8ca7683599");
    ADD_CHECKPOINT(5000, "6626e9e126927e2139ab39b3cdad75c23eceab23b3cf0a1a9255f4bbca322707");
    ADD_CHECKPOINT(5500, "5e57c11639374e249fdce113278101963de4ec36990e3157ba5138b2007f6d34");
    ADD_CHECKPOINT(6000, "7384e6dff95f477224603fbf5c0030d1f36e551531782272fcd45cc7c91a10eb");
    ADD_CHECKPOINT(6500, "54ea319fac807e793eb5a680c7e4d3de7fe6518c9924e3f26e2f7473c089a31e");
    ADD_CHECKPOINT(7000, "85ffcbd841b6b243ecc8a84e632d9164704d3ef7305ecff6fd4a7a9a812200e7");
    ADD_CHECKPOINT(7500, "e661d19e63226edcc15a46318ee3acbe8996393e304ebd7c3e8f0da238a6ef29");
    ADD_CHECKPOINT(8000, "8d6f97ff240d9c6debb4d1a318dc1ba5368b5385dd50c16ff814a9996094f988");
    ADD_CHECKPOINT(8500, "014d3e2546f45d4c47d0acb56f5737b287d155d9cd6411f7fc0da2b636615405");
    ADD_CHECKPOINT(9000, "1cb83c5b37b27af3e63f2f4e577417d258a6170039e718236e30a6a3b59853b9");
    ADD_CHECKPOINT(9500, "91f280cda9cc569f40d9918648e483eccf7d4826099668c0d97a02612b6b821f");
    ADD_CHECKPOINT(10000, "4ad536a58ef5c4e2571270421d28d440729db307af7059462eb4d19abb716ee1");
    ADD_CHECKPOINT(10500, "597307a7076fd85b3eac19bf9dcff78f7e582927e9403f52296ad42b80792cb7");
    ADD_CHECKPOINT(11000, "864dbcd76e7a43e3717766bca7c04c51175dafd7024fe47c188ddc03c9542b57");
    ADD_CHECKPOINT(11500, "a9d67f2a45b0e5e9fe2d816cba64fb8daa42de9dd21641b32f2bc6353c39c7db");
    ADD_CHECKPOINT(12000, "4785bdc52329f98508f06d5f7a19f8476638ac4eafd95b878814ba66ca78661b");
    ADD_CHECKPOINT(12500, "7346f62eaf09024395e81ed3a096cf3a6f95a0f0640a4571dff798019a161259");
    ADD_CHECKPOINT(13000, "8658ede1b029ada292f3db5fcbe03d05a5355b680be455251e991ddf96b30a32");
    ADD_CHECKPOINT(13500, "9c8d81f40018a45db858dc8cc9ac854958d7d9e4588b23e65c091e69220ccc22");
    ADD_CHECKPOINT(14000, "3be9954bf6fcfa53b025ab03a70aebd6e0f578dd9fadb0f3480ccd2c00375dfb");
    ADD_CHECKPOINT(14500, "055bc95bbfd82aa967546d494d2cbb97e5b6afa415f1d14bed25d2c8bc5e031a");
    ADD_CHECKPOINT(15727, "ce2fff2a8e66b4d416fc0cf44b9e82eb793b4629790f07be8e3ba8efd8bc6405");
    ADD_CHECKPOINT(17920, "9664327529bcb0ef2fd03b2f3eb8f7f54b89c015aef45edc77cf4812f7e1acc0");
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
