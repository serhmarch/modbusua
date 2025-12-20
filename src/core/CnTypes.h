/*!
 * \file   CnTypes.h
 * \brief  Містить список основних типів
 *
 * \author serhmarch
 * \date   September 2025
 */
#ifndef CNTYPES_H
#define CNTYPES_H

#include "CnCore.h"

#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include "CnCriticalSection.h"
#include "CnTimestamp.h"
#include "CnDateTime.h"
#include "CnString.h"
#include "CnVariant.h"

/// \brief CnList template class
template <class T>
using CnList = std::list<T>;

/// \brief CnStringList template class
using CnStringList = CnList<CnString>;

/// \brief CnSet template class
template <class T>
using CnSet = std::unordered_set<T>;

using CnByteArray = std::vector<uint8_t>;

/// \brief Cn::CnHash template class
template <class Key, class Value>
using CnHash = std::unordered_map<Key, Value>;


#endif // CNTYPES_H