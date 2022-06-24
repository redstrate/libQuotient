// SPDX-FileCopyrightText: 2019 Alexey Andreyev <aa13q@ya.ru>
// SPDX-FileCopyrightText: 2019 Kitsune Ral <Kitsune-Ral@users.sf.net>
// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "converters.h"
#include "expected.h"
#include "qolmerrors.h"

#include <QtCore/QMetaType>
#include <QtCore/QStringBuilder>

#include <array>
#include <variant>

namespace Quotient {

constexpr auto CiphertextKeyL = "ciphertext"_ls;
constexpr auto SenderKeyKeyL = "sender_key"_ls;
constexpr auto DeviceIdKeyL = "device_id"_ls;
constexpr auto SessionIdKeyL = "session_id"_ls;

constexpr auto AlgorithmKeyL = "algorithm"_ls;
constexpr auto RotationPeriodMsKeyL = "rotation_period_ms"_ls;
constexpr auto RotationPeriodMsgsKeyL = "rotation_period_msgs"_ls;

constexpr auto AlgorithmKey = "algorithm"_ls;
constexpr auto RotationPeriodMsKey = "rotation_period_ms"_ls;
constexpr auto RotationPeriodMsgsKey = "rotation_period_msgs"_ls;

constexpr auto Ed25519Key = "ed25519"_ls;
constexpr auto Curve25519Key = "curve25519"_ls;
constexpr auto SignedCurve25519Key = "signed_curve25519"_ls;

constexpr auto OlmV1Curve25519AesSha2AlgoKey = "m.olm.v1.curve25519-aes-sha2"_ls;
constexpr auto MegolmV1AesSha2AlgoKey = "m.megolm.v1.aes-sha2"_ls;

constexpr std::array SupportedAlgorithms { OlmV1Curve25519AesSha2AlgoKey,
                                           MegolmV1AesSha2AlgoKey };

inline bool isSupportedAlgorithm(const QString& algorithm)
{
    return std::find(SupportedAlgorithms.cbegin(), SupportedAlgorithms.cend(),
                     algorithm)
           != SupportedAlgorithms.cend();
}

struct Unencrypted {};
struct Encrypted {
    QByteArray key;
};

using PicklingMode = std::variant<Unencrypted, Encrypted>;

class QOlmSession;
using QOlmSessionPtr = std::unique_ptr<QOlmSession>;

class QOlmInboundGroupSession;
using QOlmInboundGroupSessionPtr = std::unique_ptr<QOlmInboundGroupSession>;

class QOlmOutboundGroupSession;
using QOlmOutboundGroupSessionPtr = std::unique_ptr<QOlmOutboundGroupSession>;

template <typename T>
using QOlmExpected = Expected<T, QOlmError>;

struct IdentityKeys
{
    QByteArray curve25519;
    QByteArray ed25519;
};

//! Struct representing the one-time keys.
struct UnsignedOneTimeKeys
{
    QHash<QString, QHash<QString, QString>> keys;

    //! Get the HashMap containing the curve25519 one-time keys.
    QHash<QString, QString> curve25519() const { return keys[Curve25519Key]; }
};

class SignedOneTimeKey {
public:
    explicit SignedOneTimeKey(const QString& unsignedKey, const QString& userId,
                              const QString& deviceId, const QString& signature)
        : payload { { "key"_ls, unsignedKey },
                    { "signatures"_ls,
                      QJsonObject {
                          { userId, QJsonObject { { "ed25519:"_ls % deviceId,
                                                    signature } } } } } }
    {}
    explicit SignedOneTimeKey(const QJsonObject& jo = {})
        : payload(jo)
    {}

    //! Unpadded Base64-encoded 32-byte Curve25519 public key
    QString key() const { return payload["key"_ls].toString(); }

    //! \brief Signatures of the key object
    //!
    //! The signature is calculated using the process described at
    //! https://spec.matrix.org/v1.3/appendices/#signing-json
    auto signatures() const
    {
        return fromJson<QHash<QString, QHash<QString, QString>>>(
            payload["signatures"_ls]);
    }

    QByteArray signature(QStringView userId, QStringView deviceId) const
    {
        return payload["signatures"_ls][userId]["ed25519:"_ls % deviceId]
            .toString()
            .toLatin1();
    }

    //! Whether the key is a fallback key
    bool isFallback() const { return payload["fallback"_ls].toBool(); }
    auto toJson() const { return payload; }
    auto toJsonForVerification() const
    {
        auto json = payload;
        json.remove("signatures"_ls);
        json.remove("unsigned"_ls);
        return QJsonDocument(json).toJson(QJsonDocument::Compact);
    }

private:
    QJsonObject payload;
};

using OneTimeKeys = QHash<QString, std::variant<QString, SignedOneTimeKey>>;

template <typename T>
class asKeyValueRange
{
public:
    asKeyValueRange(T& data)
        : m_data { data }
    {}

    auto begin() { return m_data.keyValueBegin(); }
    auto end() { return m_data.keyValueEnd(); }

private:
    T &m_data;
};
template <typename T>
asKeyValueRange(T&) -> asKeyValueRange<T>;

} // namespace Quotient

Q_DECLARE_METATYPE(Quotient::SignedOneTimeKey)
