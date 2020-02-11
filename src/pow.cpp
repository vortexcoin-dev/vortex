// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2019-2020 The Vortex developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>

#include <bignum.h>
#include <chainparams.h>
#include <util.h>

// vortex: find last block index up to pindex
const CBlockIndex *GetLastBlockIndex(const CBlockIndex *pindex, bool fProofOfStake) {
    while (pindex && pindex->pprev && (pindex->IsProofOfStake() != fProofOfStake))
        pindex = pindex->pprev;
    return pindex;
}

unsigned int GetNextTargetRequired(const CBlockIndex *pindexLast, bool fProofOfStake, const Consensus::Params &params) {
    const arith_uint256 bnProofOfWorkLimit = UintToArith256(params.powLimit);
    unsigned int nProofOfWorkLimit = bnProofOfWorkLimit.GetCompact();

    if (pindexLast == nullptr)
        return nProofOfWorkLimit; // genesis block

    const CBlockIndex *pindexPrev = GetLastBlockIndex(pindexLast, fProofOfStake);
    if (pindexPrev->pprev == nullptr)
        return nProofOfWorkLimit; // first block
    const CBlockIndex *pindexPrevPrev = GetLastBlockIndex(pindexPrev->pprev, fProofOfStake);
    if (pindexPrevPrev->pprev == nullptr)
        return nProofOfWorkLimit; // second block

    int64_t nActualSpacing = pindexPrev->GetBlockTime() - pindexPrevPrev->GetBlockTime();

    CBigNum bnNew;
    bnNew.SetCompact(pindexPrev->nBits);
    int64_t nTargetSpacing = fProofOfStake ? params.nStakeTargetSpacing : std::min(params.nTargetSpacingWorkMax, params.nStakeTargetSpacing * (1 + pindexLast->nHeight - pindexPrev->nHeight));
    int64_t nInterval = params.nTargetTimespan / nTargetSpacing;
    bnNew *= ((nInterval - 1) * nTargetSpacing + nActualSpacing + nActualSpacing);
    bnNew /= ((nInterval + 1) * nTargetSpacing);

    if (bnNew > CBigNum(params.powLimit))
        bnNew = CBigNum(params.powLimit);

    return bnNew.GetCompact();
}

bool CheckProofOfWork(const CBlockHeader *pblock, const Consensus::Params &params) {
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(pblock->nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    uint256 PoWHash;
    pblock->GetPoWHash(&PoWHash);
    if (UintToArith256(PoWHash) > bnTarget)
        return false;

    return true;
}
