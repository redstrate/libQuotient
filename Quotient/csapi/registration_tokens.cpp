// THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN

#include "registration_tokens.h"

using namespace Quotient;

auto queryToRegistrationTokenValidity(const QString& token)
{
    QUrlQuery _q;
    addParam<>(_q, u"token"_s, token);
    return _q;
}

QUrl RegistrationTokenValidityJob::makeRequestUrl(const HomeserverData& hsData, const QString& token)
{
    return BaseJob::makeRequestUrl(hsData,
                                   makePath("/_matrix/client/v1",
                                            "/register/m.login.registration_token/validity"),
                                   queryToRegistrationTokenValidity(token));
}

RegistrationTokenValidityJob::RegistrationTokenValidityJob(const QString& token)
    : BaseJob(HttpVerb::Get, u"RegistrationTokenValidityJob"_s,
              makePath("/_matrix/client/v1", "/register/m.login.registration_token/validity"),
              queryToRegistrationTokenValidity(token), {}, false)
{
    addExpectedKey(u"valid"_s);
}
