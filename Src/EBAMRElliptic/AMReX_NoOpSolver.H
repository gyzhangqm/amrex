/*
 *       {_       {__       {__{_______              {__      {__
 *      {_ __     {_ {__   {___{__    {__             {__   {__  
 *     {_  {__    {__ {__ { {__{__    {__     {__      {__ {__   
 *    {__   {__   {__  {__  {__{_ {__       {_   {__     {__     
 *   {______ {__  {__   {_  {__{__  {__    {_____ {__  {__ {__   
 *  {__       {__ {__       {__{__    {__  {_         {__   {__  
 * {__         {__{__       {__{__      {__  {____   {__      {__
 *
 */

// BVS, June 18, 2003

#ifndef _NOOPSOLVER_H_
#define _NOOPSOLVER_H_

#include "AMReX_LinearSolver.H"


namespace amrex
{

  template <class T>
  class NoOpSolver : public LinearSolver<T>
  {
  public:
    NoOpSolver()
      {
      }

    virtual ~NoOpSolver()
      {
      }

    virtual void setHomogeneous(bool a_homogeneous)
      {
      }

    virtual void define(LinearOp<T>* a_factory, bool a_homogeneous)
      {
      }

    virtual void solve(T& a_phi, const T& a_rhs);

    virtual void setConvergenceMetrics(Real a_metric,
                                       Real a_tolerance)
      {
      }
  };

  template <class T>
  void NoOpSolver<T>::solve(T& a_phi, const T& a_rhs)
  {
    //as the name implies, this function does nothing.
  }
}

#endif /*_NOOPSOLVER_H_*/
