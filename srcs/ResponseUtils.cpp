/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:51:56 by pmateo            #+#    #+#             */
/*   Updated: 2025/07/09 15:05:00 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

void	Response::initBuilders()
{
	Response::_builders[200] = &Response::Ok;
	Response::_builders[201] = &Response::Created;
	Response::_builders[202] = &Response::Accepted;
	Response::_builders[301] = &Response::MovedPermanently;
	Response::_builders[400] = &Response::BadRequest;
	Response::_builders[403] = &Response::Forbidden;
	Response::_builders[404] = &Response::NotFound;
	Response::_builders[405] = &Response::MethodNotAllowed;
	Response::_builders[411] = &Response::LengthRequired;
	Response::_builders[414] = &Response::UriTooLong;
	Response::_builders[418] = &Response::ImATeapot;
	Response::_builders[429] = &Response::TooManyRequest;
	Response::_builders[500] = &Response::InternalServerError;
	Response::_builders[501] = &Response::NotImplemented;
	Response::_builders[502] = &Response::BadGateway;
	Response::_builders[503] = &Response::ServiceUnavailable;
	Response::_builders[504] = &Response::GatewayTimeout;
	Response::_builders[505] = &Response::HttpVersionNotSupported;
}

void	Response::initContentTypes()
{
	Response::_content_types["aac"] = "audio/aac";
	Response::_content_types["abw"] = "application/x-abiword";
	Response::_content_types["arc"] = "application/octet-stream";
	Response::_content_types["avi"] = "video/x-msvideo";
	Response::_content_types["azw"] = "application/vnd.amazon.ebook";
	Response::_content_types["bin"] = "application/octet-stream";
	Response::_content_types["bmp"] = "image/bmp";
	Response::_content_types["bz"] = "application/x-bzip";
	Response::_content_types["bz2"] = "application/x-bzip2";
	Response::_content_types["csh"] = "application/x-csh";
	Response::_content_types["css"] = "text/css";
	Response::_content_types["csv"] = "text/csv";
	Response::_content_types["doc"] = "application/msword";
	Response::_content_types["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	Response::_content_types["eot"] = "application/vnd.ms-fontobject";
	Response::_content_types["epub"] = "application/epub+zip";
	Response::_content_types["gif"] = "image/gif";
	Response::_content_types["htm"] = "text/html";
	Response::_content_types["html"] = "text/html";
	Response::_content_types["ico"] = "image/x-icon";
	Response::_content_types["ics"] = "text/calendar";
	Response::_content_types["jar"] = "application/java-archive";
	Response::_content_types["jpeg"] = "image/jpeg";
	Response::_content_types["jpg"] = "image/jpeg";
	Response::_content_types["js"] = "application/javascript";
	Response::_content_types["json"] = "application/json";
	Response::_content_types["mid"] = "audio/midi";
	Response::_content_types["midi"] = "audio/midi";
	Response::_content_types["mpeg"] = "video/mpeg";
	Response::_content_types["mpkg"] = "application/vnd.apple.installer+xml";
	Response::_content_types["odp"] = "application/vnd.oasis.opendocument.presentation";
	Response::_content_types["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	Response::_content_types["odt"] = "application/vnd.oasis.opendocument.text";
	Response::_content_types["oga"] = "audio/ogg";
	Response::_content_types["ogv"] = "video/ogg";
	Response::_content_types["ogx"] = "application/ogg";
	Response::_content_types["otf"] = "font/otf";
	Response::_content_types["png"] = "image/png";
	Response::_content_types["pdf"] = "application/pdf";
	Response::_content_types["ppt"] = "application/vnd.ms-powerpoint";
	Response::_content_types["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	Response::_content_types["rar"] = "application/x-rar-compressed";
	Response::_content_types["rtf"] = "application/rtf";
	Response::_content_types["sh"] = "application/x-sh";
	Response::_content_types["svg"] = "image/svg+xml";
	Response::_content_types["swf"] = "application/x-shockwave-flash";
	Response::_content_types["tar"] = "application/x-tar";
	Response::_content_types["tif"] = "image/tiff";
	Response::_content_types["tiff"] = "image/tiff";
	Response::_content_types["ts"] = "application/typescript";
	Response::_content_types["ttf"] = "font/ttf";
	Response::_content_types["vsd"] = "application/vnd.visio";
	Response::_content_types["wav"] = "audio/x-wav";
	Response::_content_types["weba"] = "audio/webm";
	Response::_content_types["webm"] = "video/webm";
	Response::_content_types["webp"] = "image/webp";
	Response::_content_types["woff"] = "font/woff";
	Response::_content_types["woff2"] = "font/woff2";
	Response::_content_types["xhtml"] = "application/xhtml+xml";
	Response::_content_types["xls"] = "application/vnd.ms-excel";
	Response::_content_types["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	Response::_content_types["xml"] = "application/xml";
	Response::_content_types["xul"] = "application/vnd.mozilla.xul+xml";
	Response::_content_types["zip"] = "application/zip";
	Response::_content_types["3gp"] = "video/3gpp";
	Response::_content_types["3g2"] = "video/3gpp2";
	Response::_content_types[".7z"] = "application/x-7z-compressed";
}
