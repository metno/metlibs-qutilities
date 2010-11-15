/*
  libqUtilities - Diverse Qt-classes and coserver base

  $Id$

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _QLetterCommands_h
#define _QLetterCommands_h

namespace qmstrings{
  const QString vprof               = "vprof";
  const QString addimage            = "addimage";
  const QString positions           = "positions";
  const QString showpositions       = "showpositions";
  const QString hidepositions       = "hidepositions";
  const QString changeimage         = "changeimage";
  const QString showpositionname    = "showpositionname";
  const QString showpositiontext    = "showpositiontext";
  const QString enableposclick      = "enableposclick";
  const QString enableposmove       = "enableposmove";
  const QString showtext            = "showtext";
  const QString enableshowtext      = "enableshowtext";
  const QString textrequest         = "textrequest";
  const QString selectposition      = "selectposition";
  const QString settime             = "settime";
  const QString remove              = "remove";
  const QString removeclient        = "removeclient";
  const QString newclient           = "newclient";
  const QString allclients          = "allclients";
  const QString timechanged         = "timechanged";
  const QString init_HQC_params     = "init_HQC_params";
  const QString update_HQC_params   = "update_HQC_params";
  const QString select_HQC_param    = "select_HQC_param";
  const QString apply_quickmenu     = "apply_quickmenu";
  const QString station             = "station";
  const QString changeimageandtext  = "change_image_and_text";
  const QString changeimageandimage = "change_image_and_image";
  const QString seteditpositions    = "seteditpositions";
  const QString editposition        = "editposition";
  const QString copyvalue           = "copyvalue";
  const QString areas               = "areas";
  const QString selectarea          = "selectarea";
  const QString areacommand         = "areacommand";
  const QString showarea            = "showarea";
  const QString changearea          = "changearea";
  const QString deletearea          = "deletearea";
  const QString annotation          = "annotation";
  const QString changetype          = "changetype";
  const QString autoredraw          = "autoredraw";
  const QString redraw              = "redraw";
  const QString sendkey             = "sendkey";
  const QString editmode            = "editmode";
  const QString printclicked        = "printclicked";
  const QString getcurrentplotcommand = "getcurrentplotcommand";
  const QString currentplotcommand  = "currentplotcommand";
  const QString getmaparea          = "getmaparea";
  const QString maparea             = "maparea";
  const QString directory_changed   = "directory_changed";
  const QString file_changed        = "file_changed";

  const int default_id = -1000;
  const int all = -1;
  const int port = 19444;
};
#endif


