#include "fmt/audio_decoder.h"
#include "err.h"
#include "util/file_from_audio.h"

using namespace au;
using namespace au::fmt;

IDecoder::NamingStrategy AudioDecoder::naming_strategy() const
{
    return NamingStrategy::Sibling;
}

void AudioDecoder::unpack(
    const Logger &logger,
    io::File &input_file,
    const FileSaver &file_saver) const
{
    auto output_audio = decode(logger, input_file);
    auto output_file = util::file_from_audio(output_audio, input_file.path);
    // discard any directory information
    output_file->path = output_file->path.name();
    file_saver.save(std::move(output_file));
}

res::Audio AudioDecoder::decode(const Logger &logger, io::File &file) const
{
    if (!is_recognized(file))
        throw err::RecognitionError();
    file.stream.seek(0);
    return decode_impl(logger, file);
}
