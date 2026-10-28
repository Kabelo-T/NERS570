module zorder_mod
    use, intrinsic :: iso_c_binding, only: c_int
    implicit none
contains

recursive subroutine z_order(arr, N, rstart, cstart, sz, val) bind(C, name="z_order_f")
    implicit none
    integer(c_int), intent(inout) :: arr(*)
    integer(c_int), value :: N
    integer(c_int), value :: rstart
    integer(c_int), value :: cstart
    integer(c_int), value :: sz
    integer(c_int), value :: val
    integer :: base, half, quadrants

    ! base case, we are in a 2x2 cell
    if (sz == 2) then
        base = rstart*N + cstart + 1 
        arr(base)     = val              ! tl
        arr(base+1)   = val + 1          ! tr
        arr(base+N)   = val + 2          ! bl
        arr(base+N+1) = val + 3          ! br
        return
    end if

    ! recursive step
    half = sz/2
    quadrants = (sz*sz)/4

    call z_order(arr, N, rstart,      cstart,      half, val)                ! tl
    call z_order(arr, N, rstart,      cstart+half, half, val + quadrants)    ! tr
    call z_order(arr, N, rstart+half, cstart,      half, val + 2*quadrants)  ! bl
    call z_order(arr, N, rstart+half, cstart+half, half, val + 3*quadrants)  ! br
end subroutine z_order

end module zorder_mod
