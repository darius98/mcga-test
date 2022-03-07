#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include <mcga/proc.hpp>
#include "extensions/binary_stream.hpp"

namespace mcga::test {

inline auto
  makeSocketStreamingExtension(std::optional<std::string> socketPath) {
    std::unique_ptr<mcga::proc::PipeWriter> socketWriter = nullptr;
    if (socketPath.has_value()) {
        socketWriter = mcga::proc::createLocalClientSocket(socketPath.value());
    }
    auto rawWriter = [socketWriter = std::move(socketWriter)](const void* data, std::size_t size) {
        if (socketWriter) {
            socketWriter->sendBytes(static_cast<const std::uint8_t*>(data),
                                    size);
        }
    };
    using BufferedWriterT
      = mcga::proc::BufferedWriter<256, decltype(rawWriter)>;
    auto bufferedWriter = BufferedWriterT(std::move(rawWriter));

    return mcga::test::BinaryStreamExtension{std::move(bufferedWriter),
                                             [](BufferedWriterT& writer) {
                                                 writer.flush();
                                             }};
}

}  // namespace mcga::test
