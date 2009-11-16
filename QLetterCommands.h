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

#include <puTools/miString.h>

namespace qmstrings{
  const miutil::miString vprof               = "vprof";
  const miutil::miString addimage            = "addimage";
  const miutil::miString positions           = "positions";
  const miutil::miString showpositions       = "showpositions";
  const miutil::miString hidepositions       = "hidepositions";
  const miutil::miString changeimage         = "changeimage";
  const miutil::miString showpositionname    = "showpositionname";
  const miutil::miString showpositiontext    = "showpositiontext";
  const miutil::miString enableposclick      = "enableposclick";
  const miutil::miString enableposmove       = "enableposmove";
  const miutil::miString showtext            = "showtext";
  const miutil::miString enableshowtext      = "enableshowtext";
  const miutil::miString textrequest         = "textrequest";
  const miutil::miString selectposition      = "selectposition";
  const miutil::miString settime             = "settime";
  const miutil::miString remove              = "remove";
  const miutil::miString removeclient        = "removeclient";
  const miutil::miString newclient           = "newclient";
  const miutil::miString allclients          = "allclients";
  const miutil::miString timechanged         = "timechanged";
  const miutil::miString init_HQC_params     = "init_HQC_params";
  const miutil::miString update_HQC_params   = "update_HQC_params";
  const miutil::miString select_HQC_param    = "select_HQC_param";
  const miutil::miString apply_quickmenu     = "apply_quickmenu";
  const miutil::miString station             = "station";
  const miutil::miString changeimageandtext  = "change_image_and_text";
  const miutil::miString changeimageandimage = "change_image_and_image";
  const miutil::miString seteditpositions    = "seteditpositions";
  const miutil::miString editposition        = "editposition";
  const miutil::miString copyvalue           = "copyvalue";
  const miutil::miString areas               = "areas";
  const miutil::miString selectarea          = "selectarea";
  const miutil::miString areacommand         = "areacommand";
  const miutil::miString showarea            = "showarea";
  const miutil::miString changearea          = "changearea";
  const miutil::miString deletearea          = "deletearea";
  const miutil::miString annotation          = "annotation";
  const miutil::miString changetype          = "changetype";
  const miutil::miString autoredraw          = "autoredraw";
  const miutil::miString redraw              = "redraw";
  const miutil::miString sendkey             = "sendkey";
  const miutil::miString editmode            = "editmode";
  const miutil::miString printclicked        = "printclicked";

  const int default_id = -1000;
  const int all = -1;
  const int port = 19444;
};
#endif


