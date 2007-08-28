%define module_name exalt

Summary: %{module_name} module for the Enlightenment window manager
Name: e_modules-%{module_name}
Version: 0.5
Release: 0.%(date '+%Y%m%d')
License: GPL
Group: User Interface/Desktops
URL: http://www.enlightenment.org/
Source: ftp://ftp.enlightenment.org/pub/enlightenment/%{module_name}-%{version}.tar.gz
Packager: %{?_packager:%{_packager}}%{!?_packager:Atton Jonathan <watchwolf@watchwolf.fr>}
Vendor: %{?_vendorinfo:%{_vendorinfo}}%{!?_vendorinfo:The Enlightenment Project (http://www.enlightenment.org/)}
Distribution: %{?_distribution:%{_distribution}}%{!?_distribution:%{_vendor}}
BuildRequires: ecore-devel, evas-devel, esmart-devel, edje-bin
BuildRequires: edje-devel, eet-devel, enlightenment-devel >= 0.16.999
Requires: enlightenment >= 0.16.999
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
%{module_name} module for the Enlightenment window manager.

%prep
%setup -q -n %{module_name}-%{version}

%build
%{configure}
%{__make} %{?_smp_mflags} %{?mflags}

%install
%{__make} %{?mflags_install} DESTDIR=$RPM_BUILD_ROOT install
%{find_lang} %{module_name} || true > %{module_name}.lang

%clean
test "x$RPM_BUILD_ROOT" != "x/" && rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files -f %{module_name}.lang
%defattr(-, root, root)
%doc AUTHORS ChangeLog COPYING* INSTALL NEWS README
%if "%{module_name}" == "emu"
%{_bindir}/%{module_name}*
%endif
%{_libdir}/enlightenment/modules/%{module_name}*

%changelog
