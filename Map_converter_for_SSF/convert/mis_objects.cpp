#include "mis_objects.h"
#include "../Displayinfo.h"
#include "../io/wire_reader.h"
#include "../Displayinfo.h"
#include "../wire/types.h"

#include <fstream>
#include <span>
#include <vector>

namespace convert {

void mis_objects(const std::filesystem::path& mis_folder,
                 std::string_view data)
{
	std::ofstream f(mis_folder / "objs", std::ios::binary);
	if (!f)
	{
		errorWriteFile("objs");
		return;
	}

	WireReader r{std::as_bytes(std::span{data})};
	const size_t count = data.size() / sizeof(coordinates16);

	std::vector<coordinates32> dst(count);
	for (size_t i = 0; i < count; ++i)
	{
		const auto src = r.read<coordinates16>();
		dst[i].u = src.u;
		dst[i].v = src.v;
	}

	f.write(reinterpret_cast<const char*>(dst.data()),
	        static_cast<std::streamsize>(count * sizeof(coordinates32)));
}

} // namespace convert
