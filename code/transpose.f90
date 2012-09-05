module transpose
  implicit none
contains
  subroutine transpose_naive(input, output, n)
    real(8), dimension(n, n) :: input, output
    integer :: n, i, j

    do j = 1, n
       do i = 1, n
          output(j, i) = input(i, j)
       end do
    end do
  end subroutine transpose_naive

  subroutine transpose_tiled(input, output, n)
    integer, parameter :: t = 32
    real(8), dimension(n, n) :: input, output
    integer :: n, itile, jtile, i, j

    ! Note: This obviously fails if n does not divide tilesize;
    ! fixing this up is left as an exercise to the reader...
    do jtile = 0, n / t - 1
       do itile = 0, n / t - 1
          do j = 1, t
             do i = 1, t
                output(jtile * t + j, itile * t + i) = &
                     & input(itile * t + i, jtile * t + j)
             end do
          end do
       end do
    end do
  end subroutine transpose_tiled

end module transpose


program transpose_runner
  use transpose
  use omp_lib
  implicit none

  integer, parameter :: n = 8192, nrepeat = 5
  real(8) :: t0, dt_naive, dt_tiled
  real(8), dimension(nrepeat) :: timings
  real(8), dimension(:, :), allocatable :: input, output
  integer :: i, j, r

  !$omp parallel default(private)
  allocate(input(n, n))
  allocate(output(n, n))

  do j = 1, n
     do i = 1, n
        input(j, i) = (j - 1) * n + (i - 1)
     end do
  end do

  if (omp_get_thread_num() == 0) then
     write (*,*)
     write (*,*) "First two columns of input:"
     do j = 1, 10
        write (*,"(F10.0,F10.0)") input(j, 1), input(j, 2)
     end do
     write (*,*) "..."
     do j = n - 10, n
        write (*,"(F10.0,F10.0)") input(j, 1), input(j, 2)
     end do

     write (*,*)
  end if

  ! Benchmark transpose_naive
  do r = 1, nrepeat
     !$omp barrier
     t0 = omp_get_wtime()
     call transpose_naive(input, output, n)
     !$omp barrier
     timings(r) = omp_get_wtime() - t0
  end do
  dt_naive = minval(timings)
  if (omp_get_thread_num() == 0) then
     write (*,'(A,ES14.3)') "transpose_naive running time: ", dt_naive
  end if

  ! Benchmark transpose_tiled
  t0 = omp_get_wtime()
  do r = 1, nrepeat
     !$omp barrier
     t0 = omp_get_wtime()
     call transpose_tiled(input, output, n)
     !$omp barrier
     timings(r) = omp_get_wtime() - t0
  end do
  dt_tiled = minval(timings)
  
  if (omp_get_thread_num() == 0) then
     write (*,'(A,ES14.3)') "transpose_tiled running time: ", dt_tiled
     write (*,*) "speedup", dt_naive / dt_tiled

     write (*,*)
     write (*,*) "First two colums of output:"
     do j = 1, 10
        write (*,"(F10.0,F10.0)") output(j, 1), output(j, 2)
     end do
     write (*,*) "..."
     do j = n - 10, n
        write (*,"(F10.0,F10.0)") output(j, 1), output(j, 2)
     end do
  end if
  deallocate(input)
  deallocate(output)
  !$omp end parallel

end program transpose_runner
