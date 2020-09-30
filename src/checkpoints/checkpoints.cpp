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
    ADD_CHECKPOINT(200, "5c2d797d182fcb8b3e4e9ba9628a5ca9d1f054fd43ef9be004ae8746168b1cf5");
    ADD_CHECKPOINT(400, "bc579316e5fbb2f62ed899b60e0ae0b6d921a7b5e4ff61f06c9eda6805242f5e");
    ADD_CHECKPOINT(600, "f46525bd766c87fe1f7cf2dbeb5b1fefa74674c5954a523a8b3914fa6b8a935d");
    ADD_CHECKPOINT(800, "45c0e23ca8ea2e603cab157494677b347f6b87c06c4aa03633e98617a187f8e1");
    ADD_CHECKPOINT(1000, "120a7b29ae3879063246a8d415501b5577d4ac9e0f374d9a2ca99a1099b02fc4");
    ADD_CHECKPOINT(1200, "843cbe27a51220ef3d834a1bb1bd7eb6148f7711617998edf36aaa99a4976a52");
    ADD_CHECKPOINT(1400, "9756f872a98d2912e66f10327fdccd9e53d6510e132fa7556b74aa7ce48d190a");
    ADD_CHECKPOINT(1600, "4ed540146c241e36e48be2660c41c6f3235709e28c45fe12d8c1cab6a73837fc");
    ADD_CHECKPOINT(1800, "d9299efaec0102121393191a2ed4ac90aaeb854c67454fe1ec1af8aa313fcf4c");
    ADD_CHECKPOINT(2000, "a41db5720b3967e2b44926629053ad4ece169c83972df2ff80a7e2f31c8897a3");
    ADD_CHECKPOINT(2200, "771831dfb8519755e04653e7b2e99c94ab814e842ee5472b76732d0cb94196ce");
    ADD_CHECKPOINT(2400, "46e7a84584e5563ca377301a189764bfc7e83e0eab558f54f9ef10c9877dad85");
    ADD_CHECKPOINT(2600, "4856285817d3b9d69962af7c946bc0219ead1e1101913b8a60ece48fa925a60c");
    ADD_CHECKPOINT(2800, "3b1f6c24b1f50cef319bd4cc3907fba43ac440ec3008309e1aa664faf5028343");
    ADD_CHECKPOINT(3000, "a66c564ddf5c9171e57f4f054cb1ed763c41954b01b85fbd84d83c1006686155");
    ADD_CHECKPOINT(3200, "c1a10950610044a7e04692859fb78e076053d64e835bbd5ccd9a722a1c8cda42");
    ADD_CHECKPOINT(3400, "14c5384cf0a4617428d881ce272762b723d509749a15a2900bc609637f3c298b");
    ADD_CHECKPOINT(3600, "170d624fc88de4579217a2a1fb3ca6b1b86eccb516fc5be124ab1abfc7af083f");
    ADD_CHECKPOINT(3800, "fbd1fc0ae2f58735736fe17d0c4f61356b01c24b514a53c20b3898ea5058b49c");
    ADD_CHECKPOINT(4000, "7b09da7bfa0b79b53f7ca5fc8e5a1b2a7ee3b73b4083efbb5289d8993f929569");
    ADD_CHECKPOINT(4200, "3f259e670ce7a371e139b4ac75281267e5a0d8985a4fed56590495421953269c");
    ADD_CHECKPOINT(4400, "746aae6490129c9e8f359dbd60211988c50e911082aa94353cbd14fc0905265d");
    ADD_CHECKPOINT(4600, "68f43de8975fb771285f13a9554b5992ea5d7f95be0aed60aaf560b539ff35a9");
    ADD_CHECKPOINT(4800, "156f4b5db30a1afb7fdb5b8a3dd0927eba6b3581e8a4dbc63e1eddcfcb40384e");
    ADD_CHECKPOINT(5000, "e032ee8de61d5e04886cc6d98ffc8f9c5bb8f7842f63584e51b5fc98f5a69cde");
    ADD_CHECKPOINT(5200, "ddae7b2c6603ee6ff9878d37926a109834ada55c7c29d70f507b469b70b9ecd0");
    ADD_CHECKPOINT(5400, "d2d1dea2af9d6cf522ce263450152b7a3552edeb924c8ebfef7e714567c2d43a");
    ADD_CHECKPOINT(5600, "050f054ae99c3cea125e91164f0488d121c8b8681f88ca8ca6a9fe0351a2b6b6");
    ADD_CHECKPOINT(5800, "12cd73d9cf84e4c023c9917b368393931d30fb40af56b77e9b3e4405d44bde11");
    ADD_CHECKPOINT(6000, "3d7c9e1e1bcde1248618d2f7ed05d90f45032b3c67c27a15968500104c687662");
    ADD_CHECKPOINT(6200, "af06fb8f2a0303c21ff5e3d94f574290028363a904ecd13599f713de51a27c38");
    ADD_CHECKPOINT(6400, "2c381a8aa07172db6c77054bebc61620075e4fc602bb6999e090972fbcd3e6d3");
    ADD_CHECKPOINT(6600, "fe02dbdf353cd6648e78810899f9ab9dd8023b5023d181dd5c6399e09580b120");
    ADD_CHECKPOINT(6800, "c21cf558efb317244ef898eaed171e9a41c3a67185158df10ad6cbe21371c164");
    ADD_CHECKPOINT(7000, "ca8fb6d0e299bb94fa7ad51e978d15bd5b2da55ceb51f5d65562e49a619c8b4a");
    ADD_CHECKPOINT(7200, "bb0ed5a2bb9b4fffaddf98e16f000355db80233abfc0f043f095bba8c9a26b80");
    ADD_CHECKPOINT(7400, "ad6ed47b9ef3e8603992ddc04c529aa2ecb532a3ee13a7f05b1744a9a422aa1f");
    ADD_CHECKPOINT(7600, "ad5ee649b9a94361069880f2d8a7c42713618be8a6d22431a3ce420a0026f2c1");
    ADD_CHECKPOINT(7800, "6a76289f6055d5cfd81afb6c8ab69927764a76afd3eb56c5454426203f90f6a0");
    ADD_CHECKPOINT(8000, "68927a4cb667a04cf61351fb23131674126b173d75a107863b60beb03561945a");
    ADD_CHECKPOINT(8200, "0109848388f5063f1e6fbece46a6cd419b7edb6abbc6051618954120c9dd13d1");
    ADD_CHECKPOINT(8400, "c6b45b1600c6cf4c58266d5ec84e747c8738e018a159d8b0a9a815f956abd81c");
    ADD_CHECKPOINT(8600, "664fba134f13c451bc40ecce344bfad6e367a89f373ccbfe732cb22d2011a834");
    ADD_CHECKPOINT(8800, "ea0789d389c2e09b2574018cfcd7c8709afb9aebda347cf9754a8278b4777f80");
    ADD_CHECKPOINT(9000, "930398dea4ab879445db73b75f7dcd7f8fbfd61acbae24a38236719a06aedb05");
    ADD_CHECKPOINT(9200, "58f2a6c40aa85c477a62a0f81711db14d4aad466f0e719d2d21e82d8f04cc427");
    ADD_CHECKPOINT(9400, "df896fb8a92054d90a78891dd3854065011d083a43067a74a37c1319b7e5526f");
    ADD_CHECKPOINT(9600, "471c862c8c46e4dd0cf2323412916b8a14800c1f8f7d9492ade5b5cd16b5b5ed");
    ADD_CHECKPOINT(9800, "173b5fdac6d59e18fa61d157cae4b8f616e4b679c0a619839a8624341f0662f9");
    ADD_CHECKPOINT(10000, "b50899ce2815a3c61c21333979aa8102d64bf83d564c1f87fb2c259f32563ebd");
    ADD_CHECKPOINT(10200, "e7dedc8ba14dac74006a361413c755d8c5845939031d2dc169917aa3bc0b15a7");
    ADD_CHECKPOINT(10400, "43829c4df1366c7042fc5b19780f6c66463a07eb683e018f7042f85ef9d4e53f");
    ADD_CHECKPOINT(10600, "cf342cb13cf692435f89481988d1e11947df56712d41f2e4df24c83690e5c022");
    ADD_CHECKPOINT(10800, "52d69ac199cd58fcc39c9be078c56247e21cc047cfc57aadbff6eeed762dc291");
    ADD_CHECKPOINT(11000, "e5ea19e0fdf1ee32e7d2ba70a02be9b05e6f5501170b98b7e38d39080ae59f24");
    ADD_CHECKPOINT(11200, "c55a1f442e004e935cfdc7dfeb0bfd62b77f65a4b8b7c11baa8fd7972c7b9ec3");
    ADD_CHECKPOINT(11400, "6c7929afe5d44d960f0e70551ea7635cc7304bf0cca9a25a45decfd33a6c4bed");
    ADD_CHECKPOINT(11600, "3a9ac7023b9124c8bd7bf9d3a6511d913d291068c36b11673aafce88fd2a88cc");
    ADD_CHECKPOINT(11800, "52ea958e66c9d1a16c84fe987ffbfe5950a3a418b3b44953136fcc166ae69254");
    ADD_CHECKPOINT(12000, "b28b5c4eb4f30f5354ca972314457a3f8e7778b724f39a96530ecc3f4bebc895");
    ADD_CHECKPOINT(12200, "23d1bd093c2b788ced53afa897c031e5f9b0e43a7b559f5938bf8e9b9cffc1f5");
    ADD_CHECKPOINT(12400, "ad40ef3cd094c4b54d203066db27f760801cb5d03ffcd4e07243a69a2d457085");
    ADD_CHECKPOINT(12600, "cc4a19d22d44270917dde150a73eff4ebeed04fe28de056d96853d22f9f91159");
    ADD_CHECKPOINT(12800, "d5618677910672982cffab502b655154fbd5fe0af8a5e6c03ebaf7a03b54a925");
    ADD_CHECKPOINT(13000, "d39690eb3b87d7b4754132da089c4c9401e7dce66da714a9fbcbc2a43a1beda2");
    ADD_CHECKPOINT(13200, "771a4267e780cfedad28b1c3ca2e9fcbf4e51923ccdb17d772c9523e7846a76e");
    ADD_CHECKPOINT(13400, "35f41f38adbd46a4637df75c16e813a6eed4284de38382809dedd2ce54386247");
    ADD_CHECKPOINT(13600, "1e7e119f175805118c9151477db86c1a90688870b8b43f9d84f206fe9d399466");
    ADD_CHECKPOINT(13800, "77c22782c8dc3ec2f27e0696b74e884eed46eba2d4b857e4d17b322a6858ce47");
    ADD_CHECKPOINT(14000, "fdfc5f4b096fc84bbbf8459aa2577096eb123ba903c3dbc1b3a719d0be3d673a");
    ADD_CHECKPOINT(14200, "4c1d53d1854ed8c84e488e2e85c8cf909a88ab4d269951ccba27e64db6784fed");
    ADD_CHECKPOINT(14400, "cb4555354874efd2a7c588ac23a09ecb816b4f2bf34f8a0ca4321fc1c1ac9369");
    ADD_CHECKPOINT(14600, "606fd9880ce35bd86ddaa1fe20f286fbc14060c3b687b1f1aa357ae4c46013bf");
    ADD_CHECKPOINT(14800, "076b494d352c1a7b4368c9bd8168d5b807a49257e64bf2051102c8cbdf0232a1");
    ADD_CHECKPOINT(15000, "9656cf8fb12529d66bb22bb3cf97d360cca2b7a870e92045d497fe19fa1b021c");
    ADD_CHECKPOINT(15200, "b55d368e8005976d237f15060a305eace357f76d32a0ac53ea3dd0f8c111f30d");
    ADD_CHECKPOINT(15400, "c46c4c48ec3f624461854ace87b44d0717da6120063ebe097e7a75cfc775ea71");
    ADD_CHECKPOINT(15600, "6308753b34b31c0b1f5092d588f85b40e331a5437b164a0229af0b3aa6f8851b");
    ADD_CHECKPOINT(15800, "2614fb046766211059708cd4ff43ef17b211561da63c586fcd4684dba213fb22");
    ADD_CHECKPOINT(16000, "4460950caeadf815a1638d106b45aa19e599e4ef09afb0b79ddd32b19053aeda");
    ADD_CHECKPOINT(16200, "e3e9fdb540888da43fc2b48edd72cf4e3b664809d4142015cdeb6e4247177ec9");
    ADD_CHECKPOINT(16400, "a58dec3210abdf028aa68f42875324cd800da5866750bd6338f11e406a8996b1");
    ADD_CHECKPOINT(16600, "75e2b9e68287fc189041a4fe73ab9695fedb764f5dbf97df5d9c86b6d95b2876");
    ADD_CHECKPOINT(16800, "6cefeae821757611511de84cce10f739810c9cdf9564befa8fef0ce7e9854c3f");
    ADD_CHECKPOINT(17000, "0e085540bf43fd797090af1f3ebe1fc46382dd1c76d8e84e3b68d96a00c9efaf");
    ADD_CHECKPOINT(17200, "76cc4600c75ecb9f43c46e61a066a1769756f81dfcdce3cd16a8a5eed817e295");
    ADD_CHECKPOINT(17400, "efd9b4aae938bd424eddb2a1396cb34c615f4c95d5de09e6de6144a10550a13c");
    ADD_CHECKPOINT(17600, "4584ec230f30a8cb4d6fc92dcf3579497223f4c6152791c652c0bb535ce34068");
    ADD_CHECKPOINT(17800, "9ae648baac13848026366305184ba8bf9b6200a8c9c640a1dcd4d0aee300604d");
    ADD_CHECKPOINT(18000, "a9e32ec1ec81aa214086b3b6e7149423889915e0b8f7410e80604b978b8d2870");
    ADD_CHECKPOINT(18200, "afcf4f69a433a91932744e87bcc738923690bd071bbd5b2f5c7d270304a6d84b");
    ADD_CHECKPOINT(18400, "a792ae1c0389ba4ac727a6fd88b448d55ea00957c9e849c35f03412416dbdd2b");
    ADD_CHECKPOINT(18600, "a6d3a6db9d07cffcc6ee2f2c4b721a485af5f54b050a46ce326037de428643dd");
    ADD_CHECKPOINT(18800, "f917b0acae7372ae685b6df6fbf4dc59d92b81921f2bcc115fd54468e411445c");
    ADD_CHECKPOINT(19000, "d9dbb5f2d0aaf8046a598d6a6d7a059435acdfbd58758483c42feb8c23b75ea9");
    ADD_CHECKPOINT(19200, "16c350f117b25ae0e6d2dd332ac3dfca4c59d4a7167477966debe0d774eda5d8");
    ADD_CHECKPOINT(19400, "5c9a2658d9324d72fea6b99fce55ddc0aeeb735e41fb304f0ee91d7462bffe88");
    ADD_CHECKPOINT(19600, "4ac66aced177c8cf11b64989f21eeb11f486083dd1fbc12dfacea419d60224ba");
    ADD_CHECKPOINT(19800, "176b7f877e6ee1e80c8eb769f207dd45820dd1e8001b2838a4754529a0875790");
    ADD_CHECKPOINT(20000, "30255ce5cc82bc5172391dff9278f201a13f68f1e69109d2b2688eed36daa44e");
    ADD_CHECKPOINT(20200, "4c12d9350e950b13d1037a2cf6101e5ef860ebcf8d164ce55d773a9ba9c93742");
    ADD_CHECKPOINT(20400, "3161c693b82dd857393d1d812b56583fad1afe948659eb4611acd1f24a74b936");
    ADD_CHECKPOINT(20600, "58bd46af246ecf287d28e181835648b422d898f07835f6945b3d45c6284ad806");
    ADD_CHECKPOINT(20800, "2b4cbe653562611a8e483f306178c783e96938b337abbc4bb521e2da6bb1614a");
    ADD_CHECKPOINT(21000, "3e8d95fd5afdbd0929567b8a4ae333085871a7d6c6b4c596f84129ebb72e3c4c");
    ADD_CHECKPOINT(21200, "d000cb80aa36002a7913ed76c4b3e547d47449760023c5db4c73fc32c203c472");
    ADD_CHECKPOINT(21400, "73280a775c49ab177d90e82f456907612061bc10210f3854f25f6f8d080d1454");
    ADD_CHECKPOINT(21600, "74b45dd9782d66b8705a21a9f4ef496fb624baa980e00bd22177d989432510de");
    ADD_CHECKPOINT(21800, "5712dbc088e5962796e570c258366c18cde1997d5b0b948e7786f1f8c81ab193");
    ADD_CHECKPOINT(22000, "4a0dbbd5b7a97059eedcdcecf5f5050f72d7ad231ea9f7d453c26e08af7eeed4");
    ADD_CHECKPOINT(22200, "b7d756dba654cd7d73416a0bf339c1ffd12dc2d273ff8ffaca324228d3f15ac2");
    ADD_CHECKPOINT(22400, "78f6590a6fcb3c07c6c71d69596cadf8b2915566f12de809aa9b7299963cee79");
    ADD_CHECKPOINT(22600, "fa8cb569301b0840bb0ff9388ae40eb06d978e3e87341a229d1a6525c9130324");
    ADD_CHECKPOINT(22800, "0b414fee1dcf89913d7db7af64aa4eb77387b7fac2d8e4fc135abef76afba37a");
    ADD_CHECKPOINT(23000, "e629424cf05b1cf74398302c4f5701614bb16cfb9f1b0e7972336ed3bb49868e");
    ADD_CHECKPOINT(23200, "6882b08bcd221f88d29a8bdabac258665d4302d7bec585bc0415c175187a0dff");
    ADD_CHECKPOINT(23400, "35eae704e610a43e6e5c0dd44c4612bd63fbd72a7d16edca56251549a6a0d7c5");
    ADD_CHECKPOINT(23600, "9a7f90b7251ac318772ec1e2811bb95b481d05769a5c0045a6824c92ae5fb0ea");
    ADD_CHECKPOINT(23800, "824bb526f56315fc66f8e95729545b21a80bc822e792b72373b3ef01cf705592");
    ADD_CHECKPOINT(24000, "a59836232c15e044c7ac1486314f516d981e669851706c3fb49c609bdc0a5ef0");
    ADD_CHECKPOINT(24200, "c7bd8a9df7b41170f7e02104ae7526ba0b108b97f5e10fd3951a7c13054eb13f");
    ADD_CHECKPOINT(24400, "e0b3cba3708f12f43ae6541ae3d1a2a739c5a2a2034ff3dd86d6ca38248013da");
    ADD_CHECKPOINT(24600, "9b2bf3653de975ad753ff21a395344b45ad76cc3ea985294ed9a0c23571d5a14");
    ADD_CHECKPOINT(24800, "9f1046be9e7df9a73b75cd5dbda712656acda86f2b4199a36b5a1a91b90b9d19");
    ADD_CHECKPOINT(25000, "9eaa13f4f5871299f4ea81ea9f6129dd8b054b2dc1d16895b06ab585ed5daf22");
    ADD_CHECKPOINT(25200, "5b5b3db12e4ed2dd5701ba5a2e34da675eb6867352b94b2acb03a25579fb21e1");
    ADD_CHECKPOINT(25400, "d77588dc9c3e697cc414c64df5c3aa967e38eba9f9f93d44c83e70368beebef2");
    ADD_CHECKPOINT(25600, "9748baaaeae20de7fd648b39f8bd93e5151700a41f7c30497da7bbb1b5b8db0e");
    ADD_CHECKPOINT(25800, "001876617adfb51c9c2b29b18a07a919a9f282a811f2dbfae4ff3daf415f8f30");
    ADD_CHECKPOINT(26000, "aa77d96b7efd20fec6b067bdf2459554b6252b4bebf72416d470c7df0d73ff9f");
    ADD_CHECKPOINT(26200, "7da2b86e263e7ae43dc6f87f63c3543776c71e4b545014ff44d75b63bdcf54ab");
    ADD_CHECKPOINT(26400, "f84416f04e03170873a6af723185bff97769020d361f449b05e440d444d9ca17");
    ADD_CHECKPOINT(26600, "bff64f2668a87ec897093515cbb3ce78f37e27be23876a4927496b9e51211667");
    ADD_CHECKPOINT(26800, "9ea3e7a34e4e406de15accb466b226491331c8a4c03f2f5c2a3ac6f04b6d04d0");
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
