ssl_stat_ruby
=============

Ruby C Language Extension To Get SSL Certificate Information From URL

Install Dependency (Ubuntu)
===========================
::

    apt install libcurl4-openssl-dev

Installing Through RubyGems
===========================
::

	gem install ssl_stat

Ruby Sample Usage
=================
::

	require 'ssl_stat'
	info = check "https://example.com"