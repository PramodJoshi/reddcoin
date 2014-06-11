// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAIN_PARAMS_H
#define BITCOIN_CHAIN_PARAMS_H

#include "bignum.h"
#include "core.h"
#include "protocol.h"
#include "uint256.h"

#include <vector>

using namespace std;

typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

struct CDNSSeedData {
    string name, host;
    CDNSSeedData(const string &strName, const string &strHost) : name(strName), host(strHost) {}
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Network {
        MAIN,
        TESTNET,
        REGTEST,

        MAX_NETWORK_TYPES
    };

    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,
        EXT_PUBLIC_KEY,
        EXT_SECRET_KEY,

        MAX_BASE58_TYPES
    };

    const uint256& HashGenesisBlock() const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart() const { return pchMessageStart; }
    const vector<unsigned char>& AlertKey() const { return vAlertPubKey; }
    const vector<unsigned char>& DevKey() const { return vDevPubKey; }
    int GetDefaultPort() const { return nDefaultPort; }
    const CBigNum& ProofOfWorkLimit() const { return bnProofOfWorkLimit; }
    int SubsidyHalvingInterval() const { return nSubsidyHalvingInterval; }
    /* Used to check majorities for block version upgrade */
    int EnforceBlockUpgradeMajority() const { return nEnforceBlockUpgradeMajority; }
    int EnforceBlockUpgradeMajority_4() const { return nEnforceBlockUpgradeMajority_4; }
    int EnforceBlockUpgradeMajority_5() const { return nEnforceBlockUpgradeMajority_5; }
    int RejectBlockOutdatedMajority() const { return nRejectBlockOutdatedMajority; }
    int RejectBlockOutdatedMajority_4() const { return nRejectBlockOutdatedMajority_4; }
    int RejectBlockOutdatedMajority_5() const { return nRejectBlockOutdatedMajority_5; }
    int ToCheckBlockUpgradeMajority() const { return nToCheckBlockUpgradeMajority; }
    int ToCheckBlockUpgradeMajority_4() const { return nToCheckBlockUpgradeMajority_4; }
    int ToCheckBlockUpgradeMajority_5() const { return nToCheckBlockUpgradeMajority_5; }

    /* Used if GenerateBitcoins is called with a negative number of threads */
    int DefaultMinerThreads() const { return nMinerThreads; }
    const CBlock& GenesisBlock() const { return genesis; }
    bool RequireRPCPassword() const { return fRequireRPCPassword; }
    /* Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const { return fMiningRequiresPeers; }
    /* Default value for -checkmempool argument */
    bool DefaultCheckMemPool() const { return fDefaultCheckMemPool; }
    /* Allow mining of a min-difficulty block */
    bool AllowMinDifficultyBlocks() const { return fAllowMinDifficultyBlocks; }
    /* Make standard checks */
    bool RequireStandard() const { return fRequireStandard; }
    /* Make standard checks */
    bool IsTestNet() const { return fIsTestNet; }
    const string& DataDir() const { return strDataDir; }
    /* Make miner stop after a block is found. In RPC, don't return
     * until nGenProcLimit blocks are generated */
    bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }
    Network NetworkID() const { return networkID; }
    /* Return the BIP70 network string (main, test or regtest) */
    std::string NetworkIDString() const { return strNetworkID; }
    const vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const vector<CAddress>& FixedSeeds() const { return vFixedSeeds; }
    int RPCPort() const { return nRPCPort; }

    // PoSV
    const CBigNum& ProofOfStakeLimit() const { return bnProofOfStakeLimit; }
    const CBigNum& ProofOfStakeReset() const { return bnProofOfStakeReset; }
    int LastProofOfWorkHeight() const { return nLastProofOfWorkHeight; }
    unsigned int StakeMinAge() const { return nStakeMinAge; }
    unsigned int StakeMaxAge() const { return nStakeMaxAge; }
protected:
    CChainParams() {}

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    // Raw pub key bytes for the broadcast alert signing key.
    vector<unsigned char> vAlertPubKey;
    // Raw pub key bytes for the dev funding.
    vector<unsigned char> vDevPubKey;
    int nDefaultPort;
    int nRPCPort;
    CBigNum bnProofOfWorkLimit;
    int nSubsidyHalvingInterval;
    int nEnforceBlockUpgradeMajority;
    int nEnforceBlockUpgradeMajority_4;
    int nEnforceBlockUpgradeMajority_5;
    int nRejectBlockOutdatedMajority;
    int nRejectBlockOutdatedMajority_4;
    int nRejectBlockOutdatedMajority_5;
    int nToCheckBlockUpgradeMajority;
    int nToCheckBlockUpgradeMajority_4;
    int nToCheckBlockUpgradeMajority_5;
    string strDataDir;
    int nMinerThreads;
    vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    Network networkID;
    std::string strNetworkID;
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
    bool fRequireRPCPassword;
    bool fMiningRequiresPeers;
    bool fDefaultCheckMemPool;
    bool fAllowMinDifficultyBlocks;
    bool fRequireStandard;
    bool fIsTestNet;
    bool fMineBlocksOnDemand;

    // PoSV
    CBigNum bnProofOfStakeLimit;
    CBigNum bnProofOfStakeReset;
    int nLastProofOfWorkHeight;
    unsigned int nStakeMinAge;
    unsigned int nStakeMaxAge;
};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams &Params();

/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

#endif
