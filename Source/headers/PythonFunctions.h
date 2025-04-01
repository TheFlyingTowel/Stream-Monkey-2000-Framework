#pragma once

namespace SM2K
{
	namespace PY
	{
		///Tags

		TAG(Python);


		namespace py = pybind11;
		constexpr const cstring YT_EXTRACT = { R"(#Youtube video link extract
import sys
sys.path.insert(0,"./data/libs/yt-dlp")
from yt_dlp.YoutubeDL import YoutubeDL

def extract(source : str, m3u8 : bool):
	global sm2kWriteOut
	youtube_dl :  YoutubeDL = YoutubeDL()
	url_source = ()
	info = {}

	info = youtube_dl.extract_info(source, download=False)
	def grab_format(format_list, playlist = False):
			for i in range(len(format_list) - 1, -1, -1):
				if(not m3u8):
					if not 'acodec' in format_list[i]: continue
					av_is_ok = format_list[i]['acodec'] != None and format_list[i]['acodec'].lower() != 'none' and format_list[i]['vcodec'] != None and format_list[i]['vcodec'].lower() != 'none'
					size_is_ok = (format_list[i]['width'] != None and format_list[i]['height'] != None ) and  192 <= format_list[i]['width'] and format_list[i]['height'] >= 144
				else:
					av_is_ok = ('m3u8' in format_list[i]['protocol'])
					if not 'width' in format_list[i]: continue
					size_is_ok = (format_list[i]['width'] != None and format_list[i]['height'] != None ) and  192 <= format_list[i]['width'] and format_list[i]['height'] >= 144

				if av_is_ok and size_is_ok:
						return (format_list[i]['url'],)
			for i in range(len(format_list) - 1, -1, -1): # We do this if the other formats aren't there. 
				av_is_ok = ('m3u8' in format_list[i]['protocol'])
				if not 'width' in format_list[i]: continue
				size_is_ok = (format_list[i]['width'] != None and format_list[i]['height'] != None ) and  192 <= format_list[i]['width'] and format_list[i]['height'] >= 144

				if av_is_ok and size_is_ok:
						return (format_list[i]['url'],)
			return ()
	if 'entries' in info:
		for e in info['entries']:
			url_source += (e['original_url'],)
			url_source+= (e['title'],e['duration'],)
	else:
		url_source = grab_format(info['formats'])
		url_source += (info['title'],info['duration'],)
	return '\n'.join([str(x) for x in url_source])
)" };


		constexpr const cstring PYTHON_FUNCTIONS[1] = {
			YT_EXTRACT,
			//TODO: Add Python functions here ^^^
		};

		template<typename T>
		inline constexpr const int funcSize(const T& x) { return sizeof(x) / 8; };
		


#define EXE_FUNC(_name, _returnType, ...) py::globals()[#_name](__VA_ARGS__).cast<_returnType>() // Runs a loaded Python function.
#define EXTRACT_YT(_link, _useM3u8) EXE_FUNC(extract, string, _link, _useM3u8)





#define TRY_EXTRACT_YT(_link, _useM3u8, _out) \
		try{\
			_out = py::globals()["extract"](_link, _useM3u8).cast<string>();\
		}\
		catch (const std::exception& e) {\
		std::cerr << "Python error occurred: " << e.what() << std::endl;\
		}\

	}
}
