#include <string>
#include <iostream>

static bool is_hex_digit(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || \
		(c >= 'a' && c <= 'f'))
		return (true);
	return (false);
}

static int hex_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	return (0);
}

static char decode_percent(const std::string& str, size_t pos)
{
	if (pos + 2 >= str.length())
		return ('%');
	
	char c1 = str[pos + 1];
	char c2 = str[pos + 2];
	
	if (!is_hex_digit(c1) || !is_hex_digit(c2))
		return ('%');
	return (static_cast<char>(hex_to_int(c1) * 16 + hex_to_int(c2)));
}

std::string ft_traductor(const std::string& encoded_str)
{
	std::string result;
	
	for (size_t i = 0; i < encoded_str.length(); ++i)
	{
		if (encoded_str[i] == '%' && i + 1 < encoded_str.length())
		{
			char decoded_char = decode_percent(encoded_str, i);
			if (decoded_char != '%')
			{
				result += decoded_char;
				i += 2;
			}
			else
				result += encoded_str[i];
		}
		else
			result += encoded_str[i];
	}
	return (result);
}
