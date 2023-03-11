#include <service/utils/utils.hpp>

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

std::string HashMD5(const std::string &input)
{
    using boost::uuids::detail::md5;

    std::string output;

    md5 hash;
    md5::digest_type digest;

    hash.process_bytes(input.data(), input.size());
    hash.get_digest(digest);

    const int *const intDigest = reinterpret_cast<const int *>(&digest);
    boost::algorithm::hex(intDigest, intDigest + (sizeof(md5::digest_type) / sizeof(int)), std::back_inserter(output));

    return output;
}
