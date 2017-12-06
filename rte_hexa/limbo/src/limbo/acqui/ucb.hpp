#ifndef LIMBO_ACQUI_UCB_HPP
#define LIMBO_ACQUI_UCB_HPP

#include <Eigen/Core>

#include <limbo/tools/macros.hpp>

namespace limbo {
    namespace defaults {
        struct acqui_ucb {
            /// @ingroup acqui_defaults
            BO_PARAM(double, alpha, 0.5);
        };
    }
    namespace acqui {
        /** @ingroup acqui
        \rst
        Classic UCB (Upper Confidence Bound). See :cite:`brochu2010tutorial`, p. 14

          .. math::
            UCB(x) = \mu(x) + \alpha \sigma(x).

        Parameters:
          - ``double alpha``
        \endrst
        */
        template <typename Params, typename Model>
        class UCB {
        public:
            UCB(const Model& model, int iteration = 0) : _model(model) {}

            size_t dim_in() const { return _model.dim_in(); }

            size_t dim_out() const { return _model.dim_out(); }

            template <typename AggregatorFunction>
            double operator()(const Eigen::VectorXd& v, const AggregatorFunction& afun) const
            {
                Eigen::VectorXd mu;
                double sigma;
                std::tie(mu, sigma) = _model.query(v);
                return (afun(mu) + Params::acqui_ucb::alpha() * sqrt(sigma));
            }

        protected:
            const Model& _model;
        };
    }
}

#endif
