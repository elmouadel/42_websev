/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 21:39:49 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/17 01:07:57 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_HPP
#define MACROS_HPP

/******************************************************************************/
/*                               general macros                               */
/******************************************************************************/
#define BUF_SIZE 10
#define LISN_BACKLOG 1014
#define NB_OEVENTS 24
#define URL_LIMIT 1024

/******************************************************************************/
/*                         HTTP request header fields                         */
/******************************************************************************/
#define HR_METHOD   0
#define HR_URL      1
#define HR_HOST     2
#define HR_CTYP     3
#define HR_CNTLEN   4
#define HR_TENCOD   5

#define HS_STCODE   0
#define HS_CNTLEN   1
#define HS_LOCATN   2
#define HS_ALLOWD   3
// #define HS_LMODIF   4

/******************************************************************************/
/*                         HTTP response status codes                         */
/******************************************************************************/
#define SC_200 "200 ok"
#define SC_201 "201 Created"
#define SC_301 "301 Moved Permanently"
#define SC_302 "302 Found"
#define SC_304 "304 Not Modified"
#define SC_307 "307 Temporary Redirect"
#define SC_308 "308 Permanent Redirect"
#define SC_400 "400 Bad Request"
#define SC_403 "403 Forbidden"
#define SC_404 "404 Not Found"
#define SC_405 "405 Method Not Allowed"
#define SC_409 "409 Conflict"
#define SC_411 "411 Length Required"
#define SC_413 "413 Payload Too Large"
#define SC_414 "414 URI Too Long"
#define SC_415 "415 Unsupported Media Type"
#define SC_500 "500 Internal Server Error"
#define SC_501 "501 Not Implemented"
#define SC_505 "505 HTTP Version Not Supported"



#endif
