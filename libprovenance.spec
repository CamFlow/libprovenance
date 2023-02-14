Summary: CamFlow userspace library
Name: libprovenance
Version: 0.5.6
Release: 1
Group: audit/camflow
License: GPLv2
Source: %{expand:%%(pwd)}
BuildRoot: %{_topdir}/BUILD/%{name}-%{version}-%{release}

%description
%{summary}

%prep
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/lib/
mkdir -p $RPM_BUILD_ROOT/usr/include/
cd $RPM_BUILD_ROOT
cp -f %{SOURCEURL0}/src/libprovenance.so ./usr/lib/libprovenance.so
cp -f %{SOURCEURL0}/include/provenance.h ./usr/include/provenance.h
cp -f %{SOURCEURL0}/include/provenancefilter.h ./usr/include/provenancefilter.h
cp -f %{SOURCEURL0}/include/provenanceutils.h ./usr/include/provenanceutils.h
cp -f %{SOURCEURL0}/include/provenanceW3CJSON.h ./usr/include/provenanceW3CJSON.h
cp -f %{SOURCEURL0}/include/provenanceSPADEJSON.h ./usr/include/provenanceSPADEJSON.h

%clean
rm -r -f "$RPM_BUILD_ROOT"

%files
%defattr(644,root,root)
/usr/lib/libprovenance.so
/usr/include/provenance.h
/usr/include/provenancefilter.h
/usr/include/provenanceutils.h
/usr/include/provenanceW3CJSON.h
/usr/include/provenanceSPADEJSON.h

%post -p /sbin/ldconfig
