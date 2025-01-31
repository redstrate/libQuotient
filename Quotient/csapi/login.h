// THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN

#pragma once

#include <Quotient/csapi/definitions/user_identifier.h>
#include <Quotient/csapi/definitions/wellknown/full.h>

#include <Quotient/jobs/basejob.h>

namespace Quotient {

//! \brief Get the supported login types to authenticate users
//!
//! Gets the homeserver's supported login types to authenticate users. Clients
//! should pick one of these and supply it as the `type` when logging in.
class QUOTIENT_API GetLoginFlowsJob : public BaseJob {
public:
    // Inner data structures

    struct QUOTIENT_API LoginFlow {
        //! The login type. This is supplied as the `type` when
        //! logging in.
        QString type;

        //! If `type` is `m.login.token`, an optional field to indicate
        //! to the unauthenticated client that the homeserver supports
        //! the [`POST /login/get_token`](/client-server-api/#post_matrixclientv1loginget_token)
        //! endpoint. Note that supporting the endpoint does not
        //! necessarily indicate that the user attempting to log in will
        //! be able to generate such a token.
        bool getLoginToken{ false };
    };

    // Construction/destruction

    explicit GetLoginFlowsJob();

    //! \brief Construct a URL without creating a full-fledged job object
    //!
    //! This function can be used when a URL for GetLoginFlowsJob
    //! is necessary but the job itself isn't.
    static QUrl makeRequestUrl(const HomeserverData& hsData);

    // Result properties

    //! The homeserver's supported login types
    QVector<LoginFlow> flows() const { return loadFromJson<QVector<LoginFlow>>("flows"_ls); }
};

inline auto collectResponse(const GetLoginFlowsJob* job) { return job->flows(); }

template <>
struct QUOTIENT_API JsonObjectConverter<GetLoginFlowsJob::LoginFlow> {
    static void fillFrom(const QJsonObject& jo, GetLoginFlowsJob::LoginFlow& result)
    {
        fillFromJson(jo.value("type"_ls), result.type);
        fillFromJson(jo.value("get_login_token"_ls), result.getLoginToken);
    }
};

//! \brief Authenticates the user.
//!
//! Authenticates the user, and issues an access token they can
//! use to authorize themself in subsequent requests.
//!
//! If the client does not supply a `device_id`, the server must
//! auto-generate one.
//!
//! The returned access token must be associated with the `device_id`
//! supplied by the client or generated by the server. The server may
//! invalidate any access token previously associated with that device. See
//! [Relationship between access tokens and
//! devices](/client-server-api/#relationship-between-access-tokens-and-devices).
class QUOTIENT_API LoginJob : public BaseJob {
public:
    //! \param type
    //!   The login type being used.
    //!
    //!   This must be a type returned in one of the flows of the
    //!   response of the [`GET /login`](/client-server-api/#get_matrixclientv3login)
    //!   endpoint, like `m.login.password` or `m.login.token`.
    //!
    //!
    //! \param password
    //!   Required when `type` is `m.login.password`. The user's
    //!   password.
    //!
    //! \param token
    //!   Required when `type` is `m.login.token`. Part of Token-based login.
    //!
    //! \param deviceId
    //!   ID of the client device. If this does not correspond to a
    //!   known client device, a new device will be created. The given
    //!   device ID must not be the same as a
    //!   [cross-signing](/client-server-api/#cross-signing) key ID.
    //!   The server will auto-generate a device_id
    //!   if this is not specified.
    //!
    //! \param initialDeviceDisplayName
    //!   A display name to assign to the newly-created device. Ignored
    //!   if `device_id` corresponds to a known device.
    //!
    //! \param refreshToken
    //!   If true, the client supports refresh tokens.
    explicit LoginJob(const QString& type,
                      const std::optional<UserIdentifier>& identifier = std::nullopt,
                      const QString& password = {}, const QString& token = {},
                      const QString& deviceId = {}, const QString& initialDeviceDisplayName = {},
                      std::optional<bool> refreshToken = std::nullopt);

    // Result properties

    //! The fully-qualified Matrix ID for the account.
    QString userId() const { return loadFromJson<QString>("user_id"_ls); }

    //! An access token for the account.
    //! This access token can then be used to authorize other requests.
    QString accessToken() const { return loadFromJson<QString>("access_token"_ls); }

    //! A refresh token for the account. This token can be used to
    //! obtain a new access token when it expires by calling the
    //! `/refresh` endpoint.
    QString refreshToken() const { return loadFromJson<QString>("refresh_token"_ls); }

    //! The lifetime of the access token, in milliseconds. Once
    //! the access token has expired a new access token can be
    //! obtained by using the provided refresh token. If no
    //! refresh token is provided, the client will need to re-log in
    //! to obtain a new access token. If not given, the client can
    //! assume that the access token will not expire.
    std::optional<int> expiresInMs() const
    {
        return loadFromJson<std::optional<int>>("expires_in_ms"_ls);
    }

    //! ID of the logged-in device. Will be the same as the
    //! corresponding parameter in the request, if one was specified.
    QString deviceId() const { return loadFromJson<QString>("device_id"_ls); }

    //! Optional client configuration provided by the server. If present,
    //! clients SHOULD use the provided object to reconfigure themselves,
    //! optionally validating the URLs within. This object takes the same
    //! form as the one returned from .well-known autodiscovery.
    std::optional<DiscoveryInformation> wellKnown() const
    {
        return loadFromJson<std::optional<DiscoveryInformation>>("well_known"_ls);
    }

    struct Response {
        //! The fully-qualified Matrix ID for the account.
        QString userId{};

        //! An access token for the account.
        //! This access token can then be used to authorize other requests.
        QString accessToken{};

        //! A refresh token for the account. This token can be used to
        //! obtain a new access token when it expires by calling the
        //! `/refresh` endpoint.
        QString refreshToken{};

        //! The lifetime of the access token, in milliseconds. Once
        //! the access token has expired a new access token can be
        //! obtained by using the provided refresh token. If no
        //! refresh token is provided, the client will need to re-log in
        //! to obtain a new access token. If not given, the client can
        //! assume that the access token will not expire.
        std::optional<int> expiresInMs{};

        //! ID of the logged-in device. Will be the same as the
        //! corresponding parameter in the request, if one was specified.
        QString deviceId{};

        //! Optional client configuration provided by the server. If present,
        //! clients SHOULD use the provided object to reconfigure themselves,
        //! optionally validating the URLs within. This object takes the same
        //! form as the one returned from .well-known autodiscovery.
        std::optional<DiscoveryInformation> wellKnown{};
    };
};

template <std::derived_from<LoginJob> JobT>
constexpr inline auto doCollectResponse<JobT> = [](JobT* j) -> LoginJob::Response {
    return { j->userId(),      j->accessToken(), j->refreshToken(),
             j->expiresInMs(), j->deviceId(),    j->wellKnown() };
};

} // namespace Quotient
