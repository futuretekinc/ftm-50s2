stop_interface_ppp() {
	local cfg="$1"

	local proto
	config_get proto "$cfg" proto

	local link="$proto-$cfg"
	service_kill pppd "/var/run/ppp-${link}.pid"

	remove_dns "$cfg"

	local lock="/var/lock/ppp-$link"
	[ -f "$lock" ] && lock -u "$lock"
}

start_pppd() {
	local cfg="$1"; shift

	local proto
	config_get proto "$cfg" proto

	# unique link identifier
	local link="${proto:-ppp}-$cfg"

	# make sure only one pppd process is started
	lock "/var/lock/ppp-${link}"
	local pid="$(head -n1 /var/run/ppp-${link}.pid 2>/dev/null)"
	[ -d "/proc/$pid" ] && grep pppd "/proc/$pid/cmdline" 2>/dev/null >/dev/null && {
		lock -u "/var/lock/ppp-${link}"
		return 0
	}

	# Workaround: sometimes hotplug2 doesn't deliver the hotplug event for creating
	# /dev/ppp fast enough to be used here
	[ -e /dev/ppp ] || mknod /dev/ppp c 108 0

	local device
	config_get device "$cfg" device

	local username
	config_get username "$cfg" username

	local password
	config_get password "$cfg" password

	local keepalive
	config_get keepalive "$cfg" keepalive

	local connect
	config_get connect "$cfg" connect

	local disconnect
	config_get disconnect "$cfg" disconnect

	local pppd_options
	config_get pppd_options "$cfg" pppd_options

	local defaultroute
	config_get_bool defaultroute "$cfg" defaultroute 1
	[ "$defaultroute" -eq 1 ] && \
		defaultroute="defaultroute replacedefaultroute" || defaultroute="nodefaultroute"

	local interval="${keepalive##*[, ]}"
	[ "$interval" != "$keepalive" ] || interval=5

	local dns
	config_get dns "$config" dns

	local has_dns=0
	local peer_default=1
	[ -n "$dns" ] && {
		has_dns=1
		peer_default=0
	}

	local peerdns
	config_get_bool peerdns "$cfg" peerdns $peer_default

	[ "$peerdns" -eq 1 ] && {
		peerdns="usepeerdns"
	} || {
		peerdns=""
		add_dns "$cfg" $dns
	}

	local demand
	config_get demand "$cfg" demand 0

	local demandargs
	[ "$demand" -gt 0 ] && {
		demandargs="precompiled-active-filter /etc/ppp/filter demand idle $demand"
		[ "$has_dns" -eq 0 ] && add_dns "$cfg" 1.1.1.1
	} || {
		demandargs="persist"
	}

	local ipv6
	config_get_bool ipv6 "$cfg" ipv6 0
	[ "$ipv6" -eq 1 ] && ipv6="+ipv6" || ipv6=""

	start-stop-daemon -S -b -x /usr/sbin/pppd -m -p /var/run/ppp-$link.pid -- "$@" \
		${keepalive:+lcp-echo-interval $interval lcp-echo-failure ${keepalive%%[, ]*}} \
		$demandargs \
		$peerdns \
		$defaultroute \
		${username:+user "$username" password "$password"} \
		ipparam "$cfg" \
		ifname "$link" \
		${connect:+connect "$connect"} \
		${disconnect:+disconnect "$disconnect"} \
		${ipv6} \
		${pppd_options} \
		nodetach

	lock -u "/var/lock/ppp-${link}"
	
	env -i ACTION="ifup" INTERFACE="$config" DEVICE="$iface" PROTO=ppp /sbin/hotplug-call "iface" &
}

setup_interface_ppp() {
	local iface="$1"
	local config="$2"

	local device
	config_get device "$config" device

	local mtu
	config_get mtu "$config" mtu

	mtu=${mtu:-1492}
	start_pppd "$config" \
		mtu $mtu mru $mtu \
		"$device"
}

