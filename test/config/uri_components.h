// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <string>
#include <vector>

namespace salzaverde {
	struct UriComponents {
		enum Type {
			scheme = 0,
			userinfo,
			host,
			port,
			path,
			query,
			fragment,
			count
		};
		
		std::vector<std::string> operator[] (Type type) {
			return data[type];
		}
		
		UriComponents() {
			//Valid schemes contain characters, digits, ., + or -
			data[Type::scheme] = {
				"mailto:",
				"my.ftp+ssh-1:"
			};
		
			//Valid user infos can contain "-" / "." / "_" / "~"
			data[Type::userinfo] = {
				"sa.lza-ve_r~de@"
			};
		
			//Valid hosts can contain "-" / "."
			data[Type::host] = {
				"comp.infosystems.www.servers.unix",
				"salza-verde.org",
				"127.0.0.1",
				"localhost",
				"[2001:db8:2a:3256:adfe:5c0:3:6]"
			};
		
			//Valid ports contain digits
			data[Type::port] = {
				"1176"
			};
		
			//Valid paths can contain "-" / "." / "_" / "~" / "@" / "/"
			data[Type::path] = {
				"oasis:names:specification:docbook:dtd:xml:4.1.2",
				"comp.infosystems.www.servers.unix",
				"fred@example.com",
				"some/common/path",
				"+1-816-555-1212"
			};
		
			//Valid queries can contain reserved characters "?", "/"
			data[Type::query] = {
				"key=value",
				"key1=value1&key2=value2",
				"data?",
				"otherdata/"
			};
		
			//Valid fragments can contain reserved characters "?", "/"
			data[Type::fragment] = {
				"frag=value",
				"frag1=value1&frag2=value2",
				"frag_data?",
				"frag_otherdata/"
			};
		}
		
	private:
		std::map<Type, std::vector<std::string>> data;
	};
}
