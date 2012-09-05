module arithmetic_routines
  implicit none
contains
  subroutine a_plus_b(a, b, out)
    real(8), dimension(:) :: a, b, out
    out(:) = a + b
  end subroutine a_plus_b

  subroutine a_times_b(a, b, out)
    real(8), dimension(:) :: a, b, out
    out(:) = a * b
  end subroutine a_times_b

  subroutine a_times_b_plus_b(a, b, out)
    real(8), dimension(:) :: a, b, out
    out(:) = a(:) * b(:) + b(:)
  end subroutine a_times_b_plus_b
end module arithmetic_routines

program arithmetic
  use arithmetic_routines
  use omp_lib
  implicit none

  integer, parameter :: n = 20 * 1024 * 1024, nrepeat = 20
  real(8), dimension(nrepeat) :: timings
  real(8), dimension(:), allocatable :: a, b, x, tmp
  real(8) :: t0, t_sep, t_one
  integer :: i, r

  ! compute x = a * b + b in two different ways

  !$omp parallel default(private)
  allocate(a(n))
  allocate(b(n))
  allocate(x(n))
  allocate(tmp(n))

  do i = 1, n
     a(i) = i
     b(i) = 2 * i
  end do

  do r = 1, nrepeat
     !$omp barrier
     t0 = omp_get_wtime()
     call a_plus_b(a, b, tmp)
     call a_times_b(tmp, b, x)
     !$omp barrier
     timings(r) = omp_get_wtime() - t0
  end do
  t_sep = minval(timings)
  if (omp_get_thread_num() == 0) then
     write (*,"(A,ES10.2)") "Time taken separate transfers:", t_sep
  end if

  do r = 1, nrepeat
     !$omp barrier
     t0 = omp_get_wtime()
     call a_times_b_plus_b(a, b, x)
     !$omp barrier
     timings(r) = omp_get_wtime() - t0
  end do
  t_one = minval(timings)
  if (omp_get_thread_num() == 0) then
     write (*,"(A,ES10.2)") "Time taken one transfer:", t_one
     write (*,"(A,F10.2)") "Speedup", t_sep / t_one
  end if

  deallocate(a)
  deallocate(b)
  deallocate(x)
  deallocate(tmp)
  !$omp end parallel
end program arithmetic
