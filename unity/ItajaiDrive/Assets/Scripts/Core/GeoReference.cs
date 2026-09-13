using System;
using UnityEngine;

namespace ItajaiDrive.Core
{
    /// <summary>
    /// Converts WGS84 latitude/longitude into a local metric Unity frame.
    /// X points east, Z points north, Y is elevation in metres.
    /// Keep Unity transforms close to the origin instead of storing raw GPS values.
    /// </summary>
    public static class GeoReference
    {
        public const double OriginLatitude = -26.90675;
        public const double OriginLongitude = -48.65845;
        private const double EarthRadiusMetres = 6378137.0;
        private static readonly double CosOrigin = Math.Cos(OriginLatitude * Math.PI / 180.0);

        public static Vector3 ToLocal(double latitude, double longitude, float elevation = 0f)
        {
            double dLat = (latitude - OriginLatitude) * Math.PI / 180.0;
            double dLon = (longitude - OriginLongitude) * Math.PI / 180.0;
            float x = (float)(dLon * EarthRadiusMetres * CosOrigin);
            float z = (float)(dLat * EarthRadiusMetres);
            return new Vector3(x, elevation, z);
        }

        public static void ToGeo(Vector3 local, out double latitude, out double longitude)
        {
            latitude = OriginLatitude + (local.z / EarthRadiusMetres) * 180.0 / Math.PI;
            longitude = OriginLongitude + (local.x / (EarthRadiusMetres * CosOrigin)) * 180.0 / Math.PI;
        }
    }
}
