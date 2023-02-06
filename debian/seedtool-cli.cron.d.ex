#
# Regular cron jobs for the seedtool-cli package.
#
0 4	* * *	root	[ -x /usr/bin/seedtool-cli_maintenance ] && /usr/bin/seedtool-cli_maintenance
