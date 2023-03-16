#include <service/utils/utils.hpp>

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

template <typename T>
static inline std::string HashMD5(const T &input)
{
    using boost::uuids::detail::md5;
    static constexpr size_t md5Len = 32;

    md5 hash{};
    md5::digest_type digest{};

    hash.process_bytes(input.data(), input.size());
    hash.get_digest(digest);

    std::string output;
    output.reserve(md5Len);
    boost::algorithm::hex(reinterpret_cast<const int *>(&digest),
                          reinterpret_cast<const int *>(&digest) + (sizeof(md5::digest_type) / sizeof(int)),
                          std::back_inserter(output));

    return output;
}

std::string HashMD5(const std::vector<uint8_t> &input)
{
    return HashMD5<std::vector<uint8_t>>(input);
}

std::string HashMD5(const std::string &input)
{
    return HashMD5<std::string>(input);
}
