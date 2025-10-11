#include <CZ/Core/Utils/CZRegionUtils.h>
#include <CZ/skia/core/SkRegion.h>

using namespace CZ;

const SkRegion &CZRegionUtils::Empty() noexcept
{
    static SkRegion region { SkRegion() };
    return region;
}

void CZRegionUtils::ApplyTransform(SkRegion &region, SkISize size, CZTransform transform) noexcept
{
    region.op(SkIRect::MakeSize(size), SkRegion::Op::kIntersect_Op);

    if (transform == CZTransform::Normal)
        return;

    SkRegion tmp;
    SkRegion::Iterator it(region);

    switch (transform)
    {
    case CZTransform::Normal:
        return;
    case CZTransform::Flipped270:
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeXYWH(
                    size.height() - it.rect().fBottom,
                    size.width() - it.rect().fRight,
                    it.rect().fBottom - it.rect().fTop,
                    it.rect().fRight - it.rect().fLeft),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
        break;
    case CZTransform::Flipped90:
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeXYWH(
                    it.rect().fTop,
                    it.rect().fLeft,
                    it.rect().fBottom - it.rect().fTop,
                    it.rect().fRight - it.rect().fLeft),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
        break;
    case CZTransform::Flipped180:
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeXYWH(
                    it.rect().fLeft,
                    size.height() - it.rect().fBottom,
                    it.rect().fRight - it.rect().fLeft,
                    it.rect().fBottom - it.rect().fTop),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
        break;
    case CZTransform::Rotated180:
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeXYWH(
                    size.width() - it.rect().fRight,
                    size.height() - it.rect().fBottom,
                    it.rect().fRight - it.rect().fLeft,
                    it.rect().fBottom - it.rect().fTop),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
        break;
    case CZTransform::Flipped:
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeXYWH(
                    size.width() - it.rect().fRight,
                    it.rect().fTop,
                    it.rect().fRight - it.rect().fLeft,
                    it.rect().fBottom - it.rect().fTop),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
        break;
    case CZTransform::Rotated90:
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeXYWH(
                    it.rect().fTop,
                    size.width() - it.rect().fRight,
                    it.rect().fBottom - it.rect().fTop,
                    it.rect().fRight - it.rect().fLeft),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
        break;
    case CZTransform::Rotated270:
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeXYWH(
                    size.height() - it.rect().fBottom,
                    it.rect().fLeft,
                    it.rect().fBottom - it.rect().fTop,
                    it.rect().fRight - it.rect().fLeft),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
        break;
    default:
        return;
    }

    region = tmp;
}

void CZRegionUtils::Scale(SkRegion &region, Float32 xFactor, Float32 yFactor) noexcept
{
    if (xFactor == 1.f && yFactor == 1.f)
        return;

    SkRegion tmp;
    SkRegion::Iterator it(region);

    while (!it.done())
    {
        tmp.op(
            SkIRect::MakeLTRB(
                SkScalarFloorToInt(xFactor * SkScalar(it.rect().fLeft)),
                SkScalarFloorToInt(yFactor * SkScalar(it.rect().fTop)),
                SkScalarCeilToInt(xFactor * SkScalar(it.rect().fRight)),
                SkScalarCeilToInt(yFactor * SkScalar(it.rect().fBottom))),
            SkRegion::Op::kUnion_Op);
        it.next();
    }

    region = tmp;
}

void CZRegionUtils::Scale(SkRegion &region, Float32 factor) noexcept
{
    if (factor == 1.f)
        return;

    SkRegion tmp;
    SkRegion::Iterator it(region);

    if (factor == 0.5f)
    {
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeLTRB(
                    it.rect().left() >> 1,
                    it.rect().top() >> 1,
                    it.rect().right() >> 1,
                    it.rect().bottom() >> 1),
                SkRegion::Op::kUnion_Op),
            it.next();
        }
    }
    else if (factor == 2.f)
    {
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeLTRB(
                    it.rect().left() << 1,
                    it.rect().top() << 1,
                    it.rect().right() << 1,
                    it.rect().bottom() << 1),
                SkRegion::Op::kUnion_Op),
            it.next();
        }
    }
    else
    {
        while (!it.done())
        {
            tmp.op(
                SkIRect::MakeLTRB(
                    SkScalarFloorToInt(factor * SkScalar(it.rect().fLeft)),
                    SkScalarFloorToInt(factor * SkScalar(it.rect().fTop)),
                    SkScalarCeilToInt(factor * SkScalar(it.rect().fRight)),
                    SkScalarCeilToInt(factor * SkScalar(it.rect().fBottom))),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
    }

    region = tmp;
}

void CZRegionUtils::Scale(const SkRegion &src, SkRegion &dst, Float32 factor) noexcept
{
    if (&dst == &src)
    {
        Scale(dst, factor);
        return;
    }

    if (factor == 1.f)
    {
        dst = src;
        return;
    }

    dst.setEmpty();

    SkRegion::Iterator it(src);

    if (factor == 0.5f)
    {
        while (!it.done())
        {
            dst.op(
                SkIRect::MakeLTRB(
                    it.rect().left() >> 1,
                    it.rect().top() >> 1,
                    it.rect().right() >> 1,
                    it.rect().bottom() >> 1),
                SkRegion::Op::kUnion_Op),
            it.next();
        }
    }
    else if (factor == 2.f)
    {
        while (!it.done())
        {
            dst.op(
                SkIRect::MakeLTRB(
                    it.rect().left() << 1,
                    it.rect().top() << 1,
                    it.rect().right() << 1,
                    it.rect().bottom() << 1),
                SkRegion::Op::kUnion_Op),
            it.next();
        }
    }
    else
    {
        while (!it.done())
        {
            dst.op(
                SkIRect::MakeLTRB(
                    SkScalarFloorToInt(factor * SkScalar(it.rect().fLeft)),
                    SkScalarFloorToInt(factor * SkScalar(it.rect().fTop)),
                    SkScalarCeilToInt(factor * SkScalar(it.rect().fRight)),
                    SkScalarCeilToInt(factor * SkScalar(it.rect().fBottom))),
                SkRegion::Op::kUnion_Op);
            it.next();
        }
    }
}

SkPoint CZRegionUtils::ClosestPointFrom(const SkRegion &region, SkPoint point, Float32 margin) noexcept
{
    if (region.isEmpty())
        return point;

    Float32 smallestDistance { std::numeric_limits<Float32>::max() };
    SkPoint closestPoint;
    Float32 distance;
    SkPoint tmpPoint;
    UInt8 in;
    SkRegion::Iterator it(region);

    while (!it.done())
    {
        in = 0;

        if (point.x() <= it.rect().fLeft)
            tmpPoint.fX = Float32(it.rect().fLeft) + margin;
        else if (point.x() >= it.rect().fRight)
            tmpPoint.fX = Float32(it.rect().fRight) - margin;
        else
        {
            in++;
            tmpPoint.fX = point.x();
        }

        if (point.y() <= it.rect().fTop)
            tmpPoint.fY = Float32(it.rect().fTop) + margin;
        else if (point.y() >= it.rect().fBottom)
            tmpPoint.fY = Float32(it.rect().fBottom) - margin;
        else
        {
            in++;
            tmpPoint.fY = point.y();
        }

        if (in == 2)
            return tmpPoint;

        distance = SkPoint::Distance(point, tmpPoint);

        if (distance < smallestDistance)
        {
            smallestDistance = distance;
            closestPoint = tmpPoint;
        }

        it.next();
    }

    return closestPoint;
}
