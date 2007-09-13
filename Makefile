# includefile contains LOCALDIR-definition

include ../conf/$(OSTYPE).mk

LIBNAME=qUtilities
LOCALIDIR=$(LOCALDIR)/include/$(LIBNAME)

INCLUDE=-I../include -I$(LOCALDIR)/include  $(QTINCLUDE)

DEFINES=

MOCFILE=$(OBJDIR)/make.moc

LOCALOPTIONS="QTDIR=${QTDIR}" "MOCFILE=../${MOCFILE}" "MOC=${MOC}"


include ../conf/targets.mk

languages:
	if [ ! -d $(LANGDIR) ] ; then mkdir $(LANGDIR) ; fi
	cd $(SRCDIR); make $(OPTIONS) languages 
