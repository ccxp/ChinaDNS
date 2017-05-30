include $(TOPDIR)/rules.mk

PKG_NAME:=ChinaDNS
PKG_VERSION:=1.0.0
PKG_RELEASE=1


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/chinadns-$(PKG_VERSION)

PKG_FIXUP:=autoreconf
PKG_USE_MIPS16:=0
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/ChinaDNS/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=ChinaDNS
endef

define Package/ChinaDNS
  $(call Package/ChinaDNS/Default)
endef

define Package/ChinaDNS/description
A DNS forwarder that ignores incorrect(you knew it) responses.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ../src/* $(PKG_BUILD_DIR)/
endef

define Package/ChinaDNS/conffiles
/etc/chinadns_chnroute.txt
endef

define Package/ChinaDNS/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_CONF) ../chnroute.txt $(1)/etc/chinadns_chnroute.txt
	$(INSTALL_BIN) ./files/chinadns.init $(1)/etc/init.d/chinadns
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/chinadns.config $(1)/etc/config/chinadns
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/chinadns $(1)/usr/bin
	$(INSTALL_BIN) ../download_chinadns_file.sh $(1)/usr/bin
endef

$(eval $(call BuildPackage,ChinaDNS))
