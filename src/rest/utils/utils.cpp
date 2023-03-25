#include <utils/utils.hpp>

#include <Poco/Crypto/DigestEngine.h>

template <typename T>
static inline std::string HashMD5(const T &input)
{
    Poco::Crypto::DigestEngine engine("MD5");
    engine.update(input.data(), input.size());
    const Poco::DigestEngine::Digest &digest = engine.digest();

    return Poco::DigestEngine::digestToHex(digest);
}

std::string HashMD5(const std::vector<uint8_t> &input)
{
    return HashMD5<std::vector<uint8_t>>(input);
}

std::string HashMD5(const std::string &input)
{
    return HashMD5<std::string>(input);
}
