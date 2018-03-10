#b behc_node if 20 < node_pool_c
#if 0
#b bspc.c:129
#commands
#pvec4 poly->p
#printf "\n"
#pvec4 p
#printf "\n"
#end
#end
#
#b split_polys if p[0] == 1 && p[3] == 0.5

define pverts_next
	if $arg0 != $arg1
		pvert $arg0
		printf "\n"
		pverts_next $arg0->next $arg1
	end
end

define pverts
	if $arg0
		pvert $arg0
		printf "\n"
		pverts_next $arg0->next $arg0
	end
end

define pvert
	printf "{.pos = "
	pvec3 $arg0->pos
	printf ", .next = %p}", $arg0->next
end

define pvec4
	printf "{% 8.4f\t% 8.4f\t% 8.4f\t% 8.4f}", *($arg0+0), *($arg0+1), *($arg0+2), *($arg0+3)
end

define pvec3
	printf "{% 8.4f\t% 8.4f\t% 8.4f}", *($arg0+0), *($arg0+1), *($arg0+2)
end

define ppolys
	if $arg0
		printf "%p: {.used = %d, .next = %p}\n", $arg0, $arg0->used, $arg0->next
		ppoly $arg0->next
	end
end
