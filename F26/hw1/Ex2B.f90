program ex2b
    implicit none
    integer :: N, bound, length, i
    integer, allocatable :: arr(:)
    character(20) :: buf
    character(20) :: fmt

    call get_command_argument(1, buf)
    read(buf, *) N

    if (N/=2 .and. N/=4 .and. N/=8 .and. N/=16) then
        print '(A, I0)', "ERROR: N = ", N
        print '(A)', "Must be one of [2, 4, 8, 16]"
        stop 1
    end if

    bound = N*N
    allocate(arr(bound))
    call z_order(arr, N, 0, 0, N, 1)
    write(buf, '(I0)') bound
    length = len_trim(buf)
    write(fmt, '(A, I0, A)') '(*(I', length, ',:,1X))'
    write(*, '(A)', advance='no') 'A=['
    do i = 0, N-1
        if (i > 0) write(*, '(A)', advance='no') '   '
        write(*, fmt, advance='no') arr(i*N+1 : (i+1)*N)
        if (i < N-1) write(*, '(A)') ''
    end do
    write(*, '(A)') ']'

contains

recursive subroutine z_order(arr, N, rstart, cstart, sz, val)
    implicit none
    integer, intent(inout) :: arr(:)
    integer, intent(in) :: N
    integer, intent(in) :: rstart
    integer, intent(in) :: cstart
    integer, intent(in) :: sz
    integer, intent(in) :: val
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

end program ex2b
